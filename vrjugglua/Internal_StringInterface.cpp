/**	@file
        @brief	implementation

        @date
        2009-2013

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

// Internal Includes
#include "Internal_StringInterface.h"

// Library/third-party includes
// - none

// Standard includes
// - none

namespace vrjLua {
    namespace Internal {

        StringInterface::StringInterface(const std::string &device) {
            _iface.init(device);
        }

        std::string StringInterface::getStringData() {
            // see StringProxy.h in VRJuggler for getData() method
            // implementation
            return (_iface->getData());
        }

    } // end of Internal namespace
} // end of vrjLua namespace
