/**	@file	LuaRunBuffer.cpp
	@brief	implementation

	@date
	2009-2010

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

// Internal Includes
#include "LuaRunBuffer.h"
#include "VRJLuaOutput.h"

// Library includes
#include <vpr/Sync/Guard.h>
#include <vpr/IO/ObjectReader.h>
#include <vpr/IO/ObjectWriter.h>

// Standard includes
#include <fstream>

namespace vrjLua {

	inline bool LuaRunBuffer::_full() const {
		return _buf.full();
	}

	inline bool LuaRunBuffer::_empty() const {
		return _buf.empty();
	}

	inline bool LuaRunBuffer::ready() const {
		return _script.isValid();
	}

	LuaRunBuffer::LuaRunBuffer(unsigned int capacity, bool runBlocks) :
		_script(false),
		_buf(capacity),
		_maxRun(capacity),
		_runBlock(runBlocks) {

	}

	LuaRunBuffer::~LuaRunBuffer() {
#ifdef VERBOSE
		std::cout << "In destructor " << __FUNCTION__ << " at " << __FILE__ << ":" << __LINE__ << " with this=" << this << std::endl;
#endif
	}

	void LuaRunBuffer::initLua(lua_State * L) {
		_script = LuaScript(L, false);
	}

	vpr::GUID LuaRunBuffer::getGUID() {
		return vpr::GUID("00B73EB2-6A89-11DF-ABEB-0FA0DFD72085");
	}

	void LuaRunBuffer::writeObject(vpr::ObjectWriter* writer) {
		vpr::Guard<vpr::CondVar> guard(_cond, true);

		/// Set max # to run so we don't run anything added between
		/// data synchronization and postframe
		_maxRun = _buf.size();
		if (_maxRun > 0) {
			VRJLUA_MSG_START(dbgVRJLUA_BUFFER, MSG_STATUS)
			        << "Told to write objects - will write " << _maxRun
			        << VRJLUA_MSG_END(dbgVRJLUA_BUFFER, MSG_STATUS);
		}
		writer->writeUint32(_maxRun);
		for (unsigned int i = 0; i < _maxRun; ++i) {
			VRJLUA_MSG_START(dbgVRJLUA_BUFFER, MSG_STATUS)
			        << "Writing: " << _buf[i]
			        << VRJLUA_MSG_END(dbgVRJLUA_BUFFER, MSG_STATUS);
			writer->writeString(_buf[i]);
		}
		VRJLUA_MSG_START(dbgVRJLUA_BUFFER, MSG_STATUS)
		        << "Done writing."
		        << VRJLUA_MSG_END(dbgVRJLUA_BUFFER, MSG_STATUS);

	}

	void LuaRunBuffer::readObject(vpr::ObjectReader* reader) {
		vpr::Guard<vpr::CondVar> guard(_cond, true);
		_maxRun = reader->readUint32();
		if (_maxRun > 0) {
			VRJLUA_MSG_START(dbgVRJLUA_BUFFER, MSG_STATUS)
			        << "Told to read objects - will read " << _maxRun
			        << VRJLUA_MSG_END(dbgVRJLUA_BUFFER, MSG_STATUS);
		}
		_buf.clear();
		for (unsigned int i = 0; i < _maxRun; ++i) {
			// Overwrite it all.
			_buf.push_back(reader->readString());
		}
		if (_buf.size() != _maxRun) {
			VRJLUA_MSG_START(dbgVRJLUA_BUFFER, MSG_WARNING)
			        << "Upon reading serialized data, buffer sizes don't match! Remote has "
			        << _maxRun
			        << " but we have "
			        << _buf.size()
			        << VRJLUA_MSG_END(dbgVRJLUA_BUFFER, MSG_WARNING);
		}

	}


	bool LuaRunBuffer::addFile(const std::string & filename, bool blocking) {
		if (!_script.isValid()) {
			VRJLUA_MSG_START(dbgVRJLUA_BUFFER, MSG_WARNING)
			        << "in AddFile, but init not run with a valid LuaScript state!"
			        << VRJLUA_MSG_END(dbgVRJLUA_BUFFER, MSG_WARNING);
			return false;
		}
		std::ostringstream ss;
		ss << "dofile([==[" << filename << "]==])";
		return addString(ss.str(), blocking);
	}

	bool LuaRunBuffer::addString(const std::string & str, bool blocking) {
		if (!_script.isValid()) {
			VRJLUA_MSG_START(dbgVRJLUA_BUFFER, MSG_WARNING)
			        << "in AddString, but init not run with a valid LuaScript state!"
			        << VRJLUA_MSG_END(dbgVRJLUA_BUFFER, MSG_WARNING);
			return false;
		}
		vpr::Guard<vpr::CondVar> guard(_cond, blocking);
		if (guard.locked()) {

			if (!blocking) {
				// Non-blocking case: If it's full, bail out
				if (_full()) {
					VRJLUA_MSG_START(dbgVRJLUA_BUFFER, MSG_STATUS)
					        << "Buffer full, so returning false..."
					        << VRJLUA_MSG_END(dbgVRJLUA_BUFFER, MSG_STATUS);
					return false;
				}
			} else {
				// Blocking case: wait on the condition variable
				while (_full()) {
					_cond.wait();
				}
			}

			// OK, so we have space.  Put it in, increase the number,
			// and get out of this critical region!
			_buf.push_back(str);
			return true;
		} else {
			VRJLUA_MSG_START(dbgVRJLUA_BUFFER, MSG_STATUS)
			        << "Could not acquire buffer lock for producing, returning false..."
			        << VRJLUA_MSG_END(dbgVRJLUA_BUFFER, MSG_STATUS);
			return false;
		}
	}

	unsigned int LuaRunBuffer::runBuffer() {
		if (!_script.isValid()) {
			VRJLUA_MSG_START(dbgVRJLUA_BUFFER, MSG_WARNING)
			        << "in runBuffer, but init not run with a valid LuaScript state!"
			        << VRJLUA_MSG_END(dbgVRJLUA_BUFFER, MSG_WARNING);
			return 0;
		}
		if (_empty()) {
			return 0;
		}
		unsigned int n = _buf.size();
		unsigned int count(0);
		std::string current;
		VRJLUA_MSG_START(dbgVRJLUA_BUFFER, MSG_STATUS)
		        << "In runBuffer, have " << n << " to run..."
		        << VRJLUA_MSG_END(dbgVRJLUA_BUFFER, MSG_STATUS);
		for (unsigned int i = 0; i < n && !_buf.empty(); ++i) {
			{
				vpr::Guard<vpr::CondVar> guard(_cond, _runBlock);
				if (!guard.locked()) {
					VRJLUA_MSG_START(dbgVRJLUA_BUFFER, MSG_STATUS)
					        << "Could not acquire buffer lock for consuming, done for this frame..."
					        << VRJLUA_MSG_END(dbgVRJLUA_BUFFER, MSG_STATUS);
					return count; // won't do them out of order
				}
				current = _buf.front();
				_buf.pop_front();
				// Wake up anybody waiting for space
				_cond.signal();
				guard.release();
			}
			bool ret = _script.runString(current);
			if (!ret) {
				VRJLUA_MSG_START(dbgVRJLUA_BUFFER, MSG_ERROR)
				        << "Could not execute this statement: "
				        << current
				        << VRJLUA_MSG_END(dbgVRJLUA_BUFFER, MSG_ERROR);
			} else {
				count++;
			}
		}
		return count;
	}

} // end of vrjLua namespace
