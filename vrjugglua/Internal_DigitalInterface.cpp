/**	@file
	@brief	implementation

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


// Internal Includes
#include "Internal_DigitalInterface.h"

// Library/third-party includes
// - none

// Standard includes
// - none

namespace vrjLua {
	namespace Internal {

		DigitalInterface::DigitalInterface(const std::string & device) {
			_iface.init(device);
		}

		bool DigitalInterface::pressed() {
			return (_iface->getData() == gadget::Digital::TOGGLE_ON ||
			        _iface->getData() == gadget::Digital::ON);
		}

		bool DigitalInterface::justChanged() {
			return (_iface->getData() == gadget::Digital::TOGGLE_ON ||
			        _iface->getData() == gadget::Digital::TOGGLE_OFF);
		}

		bool DigitalInterface::justPressed() {
			return (_iface->getData() == gadget::Digital::TOGGLE_ON);
		}

		bool DigitalInterface::justReleased() {
			return (_iface->getData() == gadget::Digital::TOGGLE_OFF);
		}


	} // end of Internal namespace
} // end of vrjLua namespace
