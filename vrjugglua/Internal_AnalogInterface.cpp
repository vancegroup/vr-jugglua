/**	@file	Internal_AnalogInterface.cpp
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
#include "Internal_AnalogInterface.h"

// Library/third-party includes
// - none

// Standard includes
// - none

namespace vrjLua {
	namespace Internal {

		AnalogInterface::AnalogInterface(const std::string & device) {
			_iface.init(device);
		}

		double AnalogInterface::getData() {
			return (_iface->getData());
		}

		double AnalogInterface::getCentered() {
			return (_iface->getData() * 2.0) - 1.0;
		}

	} // end of Internal namespace
} // end of vrjLua namespace
