/**	@file	BindPositionInterfaceToLua.cpp
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

// Local includes
#ifndef LUABIND_COMBINED_COMPILE
#include "BindPositionInterfaceToLua.h"
#endif

// Library/third-party includes
#include <luabind/luabind.hpp>

// Standard includes
// - none

namespace vrjLua {

void bindPositionInterfaceToLua(LuaStatePtr state) {
	using namespace luabind;
	module(state.get(), "gadget") [
		class_<Internal::PositionInterface, boost::shared_ptr<Internal::PositionInterface> >("PositionInterface")
			.def(constructor<>())
			.def("init", & Internal::PositionInterface::init)
			.def("getMatrix", & Internal::PositionInterface::getMatrix)
			.def("getPosition", & Internal::PositionInterface::getPosition)
			.def("getForwardVector", & Internal::PositionInterface::getForwardVector)
	];
}

}// end of vrjLua namespace
