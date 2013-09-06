/**	@file
	@brief	implementation exposing binding function for the run buffer

	@date
	2009-2011

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

//          Copyright Iowa State University 2009-2011.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


// Local includes
#include "BindRunBufferToLua.h"
#define NEED_RUNBUFFER_LUABIND_OBJECT
#include "../SynchronizedRunBuffer.h"

// Library/third-party includes
#include <luabind/luabind.hpp>
#include <boost/shared_ptr.hpp>

// Standard includes
#ifdef VERBOSE
#include <iostream>
#endif

namespace vrjLua {
	using namespace luabind;

	void bindRunBufferToLua(lua_State * L) {
#ifdef VERBOSE
		std::cerr << "Registering vrjSync.RunBuffer object with Lua..." << std::flush << std::endl;
#endif
		module(L, "vrjSync") [
		    class_<SynchronizedRunBuffer, boost::shared_ptr<SynchronizedRunBuffer> >("RunBuffer")
		    .def(constructor<luabind::object>())
		    .def("init", &SynchronizedRunBuffer::init)
		    .def("addFile", &SynchronizedRunBuffer::addFile)
		    .def("addFile", (void(SynchronizedRunBuffer::*)(std::string const&))&SynchronizedRunBuffer::addFile)
		    .def("addString", &SynchronizedRunBuffer::addString)
		    .def("addString", (void(SynchronizedRunBuffer::*)(std::string const&))&SynchronizedRunBuffer::addString)
		    .def("runBuffer", &SynchronizedRunBuffer::runBuffer)
		    .property("local", &SynchronizedRunBuffer::isLocal)
		];

	}

}// end of vrjLua namespace
