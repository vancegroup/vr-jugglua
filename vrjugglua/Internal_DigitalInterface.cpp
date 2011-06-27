/**	@file	Internal_DigitalInterface.cpp
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

		DigitalInterface::DigitalInterface(const DigitalInterface & other) :
			_iface(other._iface) {
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
