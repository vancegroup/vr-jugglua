/**	@file	LuaScript.cpp
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
// for luaL_openlib
//#define LUA_COMPAT_OPENLIB 1

#include "LuaScript.h"

#include "BindOsgToLua.h"

#include "BindKernelToLua.h"
#include "BindPositionInterfaceToLua.h"
#include "BindOsgAppToLua.h"

#ifdef LUABIND_COMBINED_COMPILE
#include "BindOsgToLua.cpp"

#include "BindKernelToLua.cpp"
#include "BindPositionInterfaceToLua.cpp"
#include "BindOsgAppToLua.cpp"
#endif

// Library/third-party includes
extern "C" {
#include "lauxlib.h"
#include "lualib.h"
}
#include <luabind/luabind.hpp>

// Standard includes
#include <functional>
#include <iostream>

namespace vrjLua {

LuaScript::LuaScript() :
		_state(luaL_newstate(), std::ptr_fun(lua_close)) {
	// Load default Lua libs
	luaL_openlibs(_state.get());

	// Connect LuaBind to this state
	try {
		luabind::open(_state.get());
	} catch (const std::exception & e) {
		std::cerr << "Caught exception connecting luabind: " << e.what() << std::endl;
		throw;
	}

	/// @todo Extend the path here for shared libraries?
	//luabind::call_function<std::string>(_state.get(), "format", "%q", )
	//luaL_dostring(_state.get(), "package.cpath = ")

	/// Apply our bindings to this state

	// osgLua
	bindOsgToLua(_state);

	// vrjugglua
	bindKernelToLua(_state);
	bindPositionInterfaceToLua(_state);
	bindOsgAppToLua(_state);
}

LuaScript::LuaScript(const LuaScript & other) :
			_state(other._state) {
#ifdef VERBOSE
	std::cout << "**** LuaScript copy constructor invoked" << std::endl;
#endif
}

bool LuaScript::runFile(const std::string & fn) {
	/*
	int ret = luaL_dofile(_state.get(), fn.c_str());
	if (ret != 0) {
		std::cerr << "Could not run Lua file " << fn << std::endl;
	}
	*/
	try {
		luabind::call_function<void>(_state.get(), "require", fn.c_str());
	} catch (std::exception & e) {
		std::cerr << "Could not run Lua file " << fn << " - error: " << e.what() << std::endl;
		return false;
	}
	return true;
}

bool LuaScript::call(const std::string & func) {
	try {
		return luabind::call_function<bool>(_state.get(), func.c_str());
	} catch (const std::exception & e) {
		std::cerr << "Caught exception calling '" << func << "': " << e.what() << std::endl;
		throw;
	}
}


} // end of vrjLua namespace
