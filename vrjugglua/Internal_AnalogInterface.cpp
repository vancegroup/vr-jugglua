/**	@file	Internal_AnalogInterface.cpp
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
#include "Internal_AnalogInterface.h"

// Library/third-party includes
// - none

// Standard includes
// - none

namespace vrjLua {
namespace Internal {

AnalogInterface::AnalogInterface() :
	_ready(false) {
}

void AnalogInterface::init(const std::string & device) {
	_iface.init(device);
	_ready = true;
}

double AnalogInterface::getData() {
	return (_iface->getData());
}

double AnalogInterface::getCentered() {
	return (_iface->getData() * 2.0) - 1.0;
}

} // end of Internal namespace
} // end of vrjLua namespace
