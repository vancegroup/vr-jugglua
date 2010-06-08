/**	@file	SynchronizedRunBuffer.cpp
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

// Local includes
#include "SynchronizedRunBuffer.h"

// Library/third-party includes
// - none

// Standard includes
// - none

namespace vrjLua {

	SynchronizedRunBuffer::SynchronizedRunBuffer(luabind::object const& delegate) {
		/// Initialize run buffer's shared data ID
		_runBuf.init(LuaRunBuffer::getGUID());
		// Now, we have an state pointer, so we can set up the
		// run buffer as well!
		_runBuf->initLua(delegate.interpreter());
	}


	SynchronizedRunBuffer::SynchronizedRunBuffer(LuaStatePtr const& state) {
		/// Initialize run buffer's shared data ID
		_runBuf.init(LuaRunBuffer::getGUID());
		// Now, we have an state pointer, so we can set up the
		// run buffer as well!
		_runBuf->initLua(state.get());
	}

	SynchronizedRunBuffer::SynchronizedRunBuffer(LuaScript const& script) {
		/// Initialize run buffer's shared data ID
		_runBuf.init(LuaRunBuffer::getGUID());
		// Now, we have an state pointer, so we can set up the
		// run buffer as well!
		_runBuf->initLua(script.getLuaState().lock().get());
	}

	bool SynchronizedRunBuffer::isLocal() {
		return _runBuf.isLocal();
	}

	bool SynchronizedRunBuffer::addFile(std::string const& filename, bool blocking) {
		if (_runBuf.isLocal()) {
			return _runBuf->addFile(filename, blocking);
		} else {
			return false;
		}
	}

	bool SynchronizedRunBuffer::addString(std::string const& str, bool blocking) {
		if (_runBuf.isLocal()) {
			return _runBuf->addString(str, blocking);
		} else {
			return false;
		}
	}

	unsigned int SynchronizedRunBuffer::runBuffer() {
		return _runBuf->runBuffer();
	}

}// end of vrjLua namespace
