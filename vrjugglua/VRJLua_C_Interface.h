/**	@file	VRJLua_C_Interface.h
	@brief	header for the C interface to VRJuggLua

	@date
	2009-2010

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/
#pragma once
#ifndef INCLUDED_vrjugglua_VRJLua_C_Interface_h
#define INCLUDED_vrjugglua_VRJLua_C_Interface_h

#include <vrjugglua/LuaInclude.h>

LUA_C_INTERFACE_BEGIN

int luaopen_vrjugglua(lua_State *L);
int luaopen_libvrjugglua(lua_State *L);
int add_file_and_line(lua_State* L);

void set_exec_path(const char* argv0);

LUA_C_INTERFACE_END


#endif // INCLUDED_vrjugglua_VRJLua_C_Interface_h
