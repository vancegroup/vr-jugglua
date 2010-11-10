/**	@file	binding_detail/BindGadgetInterfacesToLua.cpp
	@brief	implementation of Gadgeteer device bindings

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
#include <vrjugglua/osgLuaBind.h>
#include "BindGadgetInterfacesToLua.h"
#endif

#include "../Internal_PositionInterface.h"
#include "../Internal_DigitalInterface.h"
#include "../Internal_AnalogInterface.h"

// Library/third-party includes
#include <luabind/luabind.hpp>

#include <osgLua/Value.h>



// Standard includes
#ifdef VERBOSE
#include <iostream>
#endif

namespace vrjLua {

using namespace luabind;

void bindGadgetInterfacesToLua(LuaStatePtr state) {
#ifdef VERBOSE
	std::cerr << "Registering Gadgeteer device interfaces with Lua..." << std::flush << std::endl;
#endif
	luabind::scope position = class_<Internal::PositionInterface,
			boost::shared_ptr<Internal::PositionInterface> >("PositionInterface")
					.def(constructor<const std::string &>())
					.def("getMatrix", & Internal::PositionInterface::getMatrix)
					.def("getPosition", & Internal::PositionInterface::getPosition)
					.def("getOrientation", & Internal::PositionInterface::getOrientation)
					.def("getQuatX", & Internal::PositionInterface::getQuatX)
					.def("getQuatY", & Internal::PositionInterface::getQuatY)
					.def("getQuatZ", & Internal::PositionInterface::getQuatZ)
					.def("getQuatW", & Internal::PositionInterface::getQuatW)
					.def("getForwardVector", & Internal::PositionInterface::getForwardVector);

	luabind::scope digital = class_<Internal::DigitalInterface,
				boost::shared_ptr<Internal::DigitalInterface> >("DigitalInterface")
					.def(constructor<const std::string &>())
					.def("isPressed", & Internal::DigitalInterface::isPressed)
					.def("isAChange", & Internal::DigitalInterface::isAChange)
					.def("wasJustPressed", & Internal::DigitalInterface::wasJustPressed)
					.def("wasJustReleased", & Internal::DigitalInterface::wasJustReleased);

	luabind::scope analog = class_<Internal::AnalogInterface,
			boost::shared_ptr<Internal::AnalogInterface> >("AnalogInterface")
					.def(constructor<const std::string &>())
					.def("getData", & Internal::AnalogInterface::getData)
					.def("getCentered", & Internal::AnalogInterface::getCentered);

	module(state.get(), "gadget") [
		position,
		digital,
		analog
	];
}

}// end of vrjLua namespace
