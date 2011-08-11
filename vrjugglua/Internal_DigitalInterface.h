/**	@file	Internal_DigitalInterface.h
	@brief	DigitalInterface wrapper class for LuaBind

	@date
	2009-2010

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

//          Copyright Iowa State University 2010-2011.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#pragma once
#ifndef INCLUDED_vrjugglua_Internal_DigitalInterface_h
#define INCLUDED_vrjugglua_Internal_DigitalInterface_h

// Internal Includes
// - none

// Library/third-party includes
#include <gadget/Type/DigitalInterface.h>

// Standard includes
#include <string>

namespace vrjLua {
	namespace Internal {
		class DigitalInterface {
			public:
				DigitalInterface(const std::string & device);
				DigitalInterface(const DigitalInterface & other);

				bool pressed();
				bool justChanged();

				bool justPressed();
				bool justReleased();

			protected:
				gadget::DigitalInterface _iface;
		};
	} // end of Internal namespace
} // end of vrjLua namespace

#endif // INCLUDED_vrjugglua_Internal_DigitalInterface_h
