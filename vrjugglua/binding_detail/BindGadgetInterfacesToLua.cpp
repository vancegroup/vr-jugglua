/**	@file
        @brief	implementation of Gadgeteer device bindings

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

// Local includes
#include "BindGadgetInterfacesToLua.h"
#include "../osgLuaBind.h"

#include "../Internal_PositionInterface.h"
#include "../Internal_DigitalInterface.h"
#include "../Internal_AnalogInterface.h"
#include "../Internal_StringInterface.h"

// Library/third-party includes
#include <luabind/class.hpp>

#include <osgLua/Value>
#include <osg/Quat>

// Standard includes
// - none

namespace vrjLua {

    using namespace luabind;

    void bindGadgetInterfacesToLua(lua_State *L) {
        luabind::scope position =
            class_<Internal::PositionInterface,
                   boost::shared_ptr<Internal::PositionInterface> >(
                "PositionInterface")
                .def(constructor<const std::string &>())
                .property("matrix", &Internal::PositionInterface::getMatrix)
                .property("position", &Internal::PositionInterface::getPosition)
                .property("orientation",
                          &Internal::PositionInterface::getOrientation)
                .property("forwardVector",
                          &Internal::PositionInterface::getForwardVector);

        luabind::scope digital =
            class_<Internal::DigitalInterface,
                   boost::shared_ptr<Internal::DigitalInterface> >(
                "DigitalInterface")
                .def(constructor<const std::string &>())
                .property("pressed", &Internal::DigitalInterface::pressed)
                .property("justChanged",
                          &Internal::DigitalInterface::justChanged)
                .property("justPressed",
                          &Internal::DigitalInterface::justPressed)
                .property("justReleased",
                          &Internal::DigitalInterface::justReleased);

        luabind::scope analog =
            class_<Internal::AnalogInterface,
                   boost::shared_ptr<Internal::AnalogInterface> >(
                "AnalogInterface")
                .def(constructor<const std::string &>())
                .property("data", &Internal::AnalogInterface::getData)
                .property("centered", &Internal::AnalogInterface::getCentered);

        luabind::scope string =
            class_<Internal::StringInterface,
                   boost::shared_ptr<Internal::StringInterface> >(
                "StringInterface")
                .def(constructor<const std::string &>())
                .property("data", &Internal::StringInterface::getStringData);

        module(L, "gadget")[position, digital, analog, string];
    }

} // end of vrjLua namespace
