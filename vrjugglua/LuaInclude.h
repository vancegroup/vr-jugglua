/**	@file	LuaInclude.h
	@brief	header

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
#ifndef INCLUDED_vrjugglua_LuaInclude_h
#define INCLUDED_vrjugglua_LuaInclude_h

#if defined(__cplusplus) && !defined(BUILD_LUA_AS_CPP)
#define LUA_C_INTERFACE_BEGIN extern "C" {
#define LUA_C_INTERFACE_END }
#else
#define LUA_C_INTERFACE_BEGIN
#define LUA_C_INTERFACE_END
#endif

LUA_C_INTERFACE_BEGIN

#include <lua.h>

LUA_C_INTERFACE_END


#endif // INCLUDED_vrjugglua_LuaInclude_h
