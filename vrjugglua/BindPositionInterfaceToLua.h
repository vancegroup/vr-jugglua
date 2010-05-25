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
#include "Lua_PositionInterface.h"
#include "LuaScript.h"

// Library/third-party includes
#include <luabind/luabind.hpp>

// Standard includes
// - none

namespace vrjLua {

void bindPositionInterfaceToLua(LuaStatePtr state) {
	using namespace luabind;
	module(state.get(), "gadget") [
		class_<PositionInterface, boost::shared_ptr<PositionInterface> >("PositionInterface")
			.def(constructor<>())
			.def("init", &PositionInterface::init)
			.def("getMatrix", & PositionInterface::getMatrix)
			.def("getPosition", & PositionInterface::getPosition)
			.def("getForwardVector", & PositionInterface::getForwardVector)
	];
}

}// end of vrjLua namespace


#endif // INCLUDED_vrjugglua_BindPositionInterfaceToLua_h
