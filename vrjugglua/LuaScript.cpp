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
#include "BindGadgetInterfacesToLua.h"
#include "BindOsgAppToLua.h"

#include "OsgAppProxy.h"

#ifdef LUABIND_COMBINED_COMPILE
#include "BindOsgToLua.cpp"

#include "BindKernelToLua.cpp"
#include "BindGadgetInterfacesToLua.cpp"
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

// Functions to register us from within a Lua interpreter
extern "C" {
	int luaopen_vrjugglua(lua_State *L);
	int luaopen_libvrjugglua(lua_State *L);
}


int luaopen_vrjugglua(lua_State *L) {
	vrjLua::LuaScript script(L);
	return 0; // success
}

int luaopen_libvrjugglua(lua_State *L) {
	return luaopen_vrjugglua(L);
}

namespace vrjLua {

/// @brief no-op deleter for externally-provided state pointers
static void no_op_deleter(lua_State *L) {
	return;
}

/// @brief Replacement, juggler-compatible print statement
static void print(const std::string & s) {

}

LuaScript::LuaScript() :
		_state(luaL_newstate(), std::ptr_fun(lua_close)) {
	// Load default Lua libs
	luaL_openlibs(_state.get());



	/// @todo Extend the path here for shared libraries?
	//luabind::call_function<std::string>(_state.get(), "format", "%q", )
	//luaL_dostring(_state.get(), "package.cpath = ")

	_applyBindings();
}

LuaScript::LuaScript(lua_State * state) :
			_state(state, std::ptr_fun(no_op_deleter)) {
	_applyBindings();
}

LuaScript::LuaScript(const LuaScript & other) :
			_state(other._state) {
#ifdef VERBOSE
	std::cout << "**** LuaScript copy constructor invoked" << std::endl;
#endif
}

bool LuaScript::runFile(const std::string & fn) {

	int ret = luaL_dofile(_state.get(), fn.c_str());
	if (ret != 0) {
		std::cerr << "Could not run Lua file " << fn << std::endl;
	}
	/*
	try {
		luabind::call_function<void>(_state.get(), "require", fn.c_str());
	} catch (std::exception & e) {
		std::cerr << "Could not run Lua file " << fn << " - error: " << e.what() << std::endl;
		return false;
	}
	*/
	return true;
}

void LuaScript::_applyBindings() {
	// Connect LuaBind to this state
	try {
		luabind::open(_state.get());
	} catch (const std::exception & e) {
		std::cerr << "Caught exception connecting luabind: " << e.what() << std::endl;
		throw;
	}

	/// Apply our bindings to this state

	// osgLua
	bindOsgToLua(_state);

	// vrjugglua
	bindKernelToLua(_state);
	bindGadgetInterfacesToLua(_state);
	bindOsgAppToLua(_state);

	OsgAppProxy::bindToLua(_state);
}

bool LuaScript::call(const std::string & func) {
	try {
		return luabind::call_function<bool>(_state.get(), func.c_str());
	} catch (const std::exception & e) {
		std::cerr << "Caught exception calling '" << func << "': " << e.what() << std::endl;
		throw;
	}
}


LuaStateWeakPtr LuaScript::getLuaState() const {
	return _state;
}

} // end of vrjLua namespace
