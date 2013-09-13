/**	@file
        @brief	header for the C interface to VRJuggLua

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

#pragma once
#ifndef INCLUDED_vrjugglua_VRJLua_C_Interface_h
#define INCLUDED_vrjugglua_VRJLua_C_Interface_h

#include "LuaStateFwd.h"

#include "LuaCInterfaceBegin.h"
int luaopen_vrjugglua(lua_State *L);
int luaopen_libvrjugglua(lua_State *L);

void set_exec_path(const char *argv0);

#include "LuaCInterfaceEnd.h"

#endif // INCLUDED_vrjugglua_VRJLua_C_Interface_h
