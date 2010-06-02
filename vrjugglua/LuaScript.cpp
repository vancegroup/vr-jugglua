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
#include "LuaScript.h"

#include "OsgAppProxy.h"
#include "LuaPath.h"

#ifdef LUABIND_COMBINED_COMPILE

#	include "BindOsgToLua.cpp"

#	include "BindKernelToLua.cpp"
#	include "BindGadgetInterfacesToLua.cpp"
#	include "BindRunBufferToLua.cpp"

#else

#	include "BindOsgToLua.h"

#	include "BindKernelToLua.h"
#	include "BindGadgetInterfacesToLua.h"
#	include "BindRunBufferToLua.h"

#endif

// Library/third-party includes
#include <vrjugglua/LuaInclude.h>

#include <luabind/luabind.hpp>

// Standard includes
#include <functional>
#include <iostream>
#include <sstream>

namespace vrjLua {

/// @brief no-op deleter for externally-provided state pointers
static void no_op_deleter(lua_State *L) {
	return;
}

/// @name Manipulating global interpreter variable
/// @{

/// External global variable - ick!
extern LuaStatePtr g_state;

void setInteractiveInterpreter(LuaStatePtr state) {
	g_state = state;
}

LuaStatePtr getInteractiveInterpreter() {
	return g_state;
}
/// @}

LuaScript::LuaScript() :
		_state(luaL_newstate(), std::ptr_fun(lua_close)) {

	lua_gc(_state.get(), LUA_GCSTOP, 0);  /* stop collector during initialization */
	// Load default Lua libs
	luaL_openlibs(_state.get());

	/// @todo Extend the path here for shared libraries?
	//luabind::call_function<std::string>(_state.get(), "format", "%q", )
	//luaL_dostring(_state.get(), "package.cpath = ")

	_applyBindings();
	lua_gc(_state.get(), LUA_GCRESTART, 0);
}

LuaScript::LuaScript(lua_State * state, bool bind) :
			_state(state, std::ptr_fun(no_op_deleter)) {
	// If requested, bind.
	if (bind) {
		_applyBindings();
	}
}

LuaScript::LuaScript(const LuaScript & other) :
			_state(other._state) {
#ifdef VERBOSE
	std::cout << "**** LuaScript copy constructor invoked" << std::endl;
#endif
}

LuaScript::LuaScript(const LuaStatePtr & otherptr) :
		_state(otherptr) {
}

LuaScript & LuaScript::operator=(const LuaScript & other) {
	_state = other._state;
	return *this;
}

bool LuaScript::runFile(const std::string & fn) {
	int ret = luaL_dofile(_state.get(), fn.c_str());
	if (ret != 0) {
		std::cerr << "Could not run Lua file " << fn << std::endl;
		return false;
	}
	return true;
}

bool LuaScript::requireModule(const std::string & mod) {
	try {
		luabind::call_function<void>(_state.get(), "require", mod);
	} catch (std::exception & e) {
		std::cerr << "Could not load Lua module " << mod << " - error: " << e.what() << std::endl;
		return false;
	}
	return true;
}

bool LuaScript::runString(const std::string & str) {

	int ret = luaL_dostring(_state.get(), str.c_str());
	if (ret != 0) {
		std::cerr << "Could not run provided Lua string" << std::endl;
		return false;
	}
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

	// Extend the lua script search path for "require"
	LuaPath lp;
	lp.prependLuaRequirePath(_state);

	// osgLua
	bindOsgToLua(_state);

	// vrjugglua
	bindKernelToLua(_state);
	bindGadgetInterfacesToLua(_state);
	bindRunBufferToLua(_state);

	OsgAppProxy::bindToLua(_state);

	// Set up traceback...
	luabind::set_pcall_callback(&add_file_and_line);
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
