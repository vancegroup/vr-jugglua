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
					.property("matrix", & Internal::PositionInterface::getMatrix)
					.property("position", & Internal::PositionInterface::getPosition)
					.property("orientation", & Internal::PositionInterface::getOrientation)
					.property("forwardVector", & Internal::PositionInterface::getForwardVector);

	luabind::scope digital = class_<Internal::DigitalInterface,
				boost::shared_ptr<Internal::DigitalInterface> >("DigitalInterface")
					.def(constructor<const std::string &>())
					.property("isPressed", & Internal::DigitalInterface::isPressed)
					.property("isAChange", & Internal::DigitalInterface::isAChange)
					.property("wasJustPressed", & Internal::DigitalInterface::wasJustPressed)
					.property("wasJustReleased", & Internal::DigitalInterface::wasJustReleased);

	luabind::scope analog = class_<Internal::AnalogInterface,
			boost::shared_ptr<Internal::AnalogInterface> >("AnalogInterface")
					.def(constructor<const std::string &>())
					.property("data", & Internal::AnalogInterface::getData)
					.property("centered", & Internal::AnalogInterface::getCentered);

	module(state.get(), "gadget") [
		position,
		digital,
		analog
	];
}

}// end of vrjLua namespace
