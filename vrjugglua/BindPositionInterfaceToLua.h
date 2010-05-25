/**	@file	BindPositionInterfaceToLua.h
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
#ifndef INCLUDED_vrjugglua_BindPositionInterfaceToLua_h
#define INCLUDED_vrjugglua_BindPositionInterfaceToLua_h

// Local includes
#include "LuaScript.h"
#include "Internal_PositionInterface.h"

// Library/third-party includes
#include <luabind/luabind.hpp>

// Standard includes
// - none

namespace vrjLua {

	void bindPositionInterfaceToLua(LuaStatePtr state);

}// end of vrjLua namespace

#ifdef LUABIND_COMBINED_COMPILE
#include "BindPositionInterfaceToLua.cpp"
#endif

#endif // INCLUDED_vrjugglua_BindPositionInterfaceToLua_h
