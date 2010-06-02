/**	@file	BindRunBufferToLua.cpp
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
#include "BindRunBufferToLua.h"

#include "LuaRunBuffer.h"

// Library/third-party includes
#include <luabind/luabind.hpp>

#include <plugins/ApplicationDataManager/UserData.h>

// Standard includes
#ifdef VERBOSE
#include <iostream>
#endif

namespace vrjLua {
using namespace luabind;

namespace Internal {
	class SynchronizedRunBuffer {
		public:
			SynchronizedRunBuffer(luabind::object const& delegate);
			bool addFile(const std::string & filename, bool blocking = false);
			bool addString(const std::string & str, bool blocking = false);
			unsigned int runBuffer();
		protected:
			cluster::UserData<LuaRunBuffer> _runBuf;
	};

	SynchronizedRunBuffer::SynchronizedRunBuffer(luabind::object const& delegate) {
		/// Initialize run buffer's shared data ID
		_runBuf.init(LuaRunBuffer::getGUID());
		// Now, we have an state pointer, so we can set up the
		// run buffer as well!
		_runBuf->initLua(LuaScript(delegate.interpreter()));
	}
	bool SynchronizedRunBuffer::addFile(const std::string & filename, bool blocking) {
		return _runBuf->addFile(filename, blocking);
	}
	bool SynchronizedRunBuffer::addString(const std::string & str, bool blocking) {
		return _runBuf->addString(str, blocking);
	}
	unsigned int SynchronizedRunBuffer::runBuffer() {
		return _runBuf->runBuffer();
	}

} // end of Internal namespace

void bindRunBufferToLua(LuaStatePtr state) {
#ifdef VERBOSE
	std::cerr << "Registering vrjSync.RunBuffer object with Lua..." << std::flush << std::endl;
#endif
	module(state.get(), "vrjSync") [
		class_<Internal::SynchronizedRunBuffer>("RunBuffer")
			.def(constructor<luabind::object>())
			.def("addFile", &Internal::SynchronizedRunBuffer::addFile)
			.def("addString", &Internal::SynchronizedRunBuffer::addString)
			.def("runBuffer", &Internal::SynchronizedRunBuffer::runBuffer)
	];

}

}// end of vrjLua namespace
