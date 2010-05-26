/**	@file	BindGadgetInterfacesToLua.cpp
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
#include "BindGadgetInterfacesToLua.h"
#endif

// Library/third-party includes
#include <luabind/luabind.hpp>

// Standard includes
#ifdef VERBOSE
#include <iostream>
#endif

namespace vrjLua {

void bindGadgetInterfacesToLua(LuaStatePtr state) {
	using namespace luabind;
#ifdef VERBOSE
	std::cerr << "Registering gadget.PositionInterface with Lua..." << std::flush << std::endl;
#endif
	module(state.get(), "gadget") [
		class_<Internal::PositionInterface, boost::shared_ptr<Internal::PositionInterface> >("PositionInterface")
			.def(constructor<>())
			.def("init", & Internal::PositionInterface::init)
			.def("getMatrix", & Internal::PositionInterface::getMatrix)
			.def("getPosition", & Internal::PositionInterface::getPosition)
			.def("getForwardVector", & Internal::PositionInterface::getForwardVector),
		class_<Internal::DigitalInterface, boost::shared_ptr<Internal::DigitalInterface> >("ButtonInterface")
			.def(constructor<>())
			.def("init", & Internal::DigitalInterface::init)
			.def("isPressed", & Internal::DigitalInterface::isPressed)
			.def("isAChange", & Internal::DigitalInterface::isAChange)
			.def("wasJustPressed", & Internal::DigitalInterface::wasJustPressed)
			.def("wasJustReleased", & Internal::DigitalInterface::wasJustReleased)
	];
}

}// end of vrjLua namespace
