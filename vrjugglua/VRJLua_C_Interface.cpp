/**	@file	VRJLua_C_Interface.cpp
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
#include "VRJLua_C_Interface.h"

#include "LuaScript.h"

// Library/third-party includes
#include <vrjugglua/LuaInclude.h>

// Standard includes
#include <functional>
#include <iostream>
#include <sstream>

int luaopen_vrjugglua(lua_State *L) {
	// Create a script and load the bindings
	vrjLua::LuaScript script(L, true);
	return 0; // success
}

int luaopen_libvrjugglua(lua_State *L) {
	return luaopen_vrjugglua(L);
}

// From the Luabind docs
int add_file_and_line(lua_State* L)
{
	lua_Debug d;
	lua_getstack(L, 1, &d);
	lua_getinfo(L, "Sln", &d);
	std::string err = lua_tostring(L, -1);
	lua_pop(L, 1);
	std::stringstream msg;
	msg << d.short_src << ":" << d.currentline;

	if (d.name != 0)
	{
		msg << "(" << d.namewhat << " " << d.name << ")";
	}
	msg << " " << err;
	lua_pushstring(L, msg.str().c_str());
	std::cerr << std::endl << msg.str() << std::endl;
	return 1;
}

namespace vrjLua {
/// @brief no-op deleter for externally-provided state pointers
static void no_op_deleter(lua_State *L) {
	return;
}

/// @name Manipulating global interpreter variable
/// @{
LuaStatePtr g_state;
/// @}

} // end of namespace vrjLua

void setInteractiveInterpreter(lua_State * state) {
	vrjLua::g_state = vrjLua::LuaStatePtr(state, std::ptr_fun(vrjLua::no_op_deleter));
}

