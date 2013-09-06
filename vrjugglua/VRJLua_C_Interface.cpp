/**	@file
	@brief	implementation

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


// Internal Includes
#include "VRJLua_C_Interface.h"

#include "ApplyBinding.h"
#include "LuaPath.h"

// Library/third-party includes
#include <vrjugglua/LuaIncludeFull.h>

// Standard includes
// - none

int luaopen_vrjugglua(lua_State *L) {
	try {
		// Load the bindings
		vrjLua::applyBinding(L);
	} catch (std::exception & e) {
		return luaL_error(L, e.what());
	}
	return 0; // success
}

int luaopen_libvrjugglua(lua_State *L) {
	return luaopen_vrjugglua(L);
}

void set_exec_path(const char* argv0) {
	vrjLua::LuaPath::instance(argv0);
}
