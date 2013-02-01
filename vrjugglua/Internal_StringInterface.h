/**	@file
	@brief	StringInterface wrapper class for LuaBind

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


#pragma once
#ifndef INCLUDED_vrjugglua_Internal_StringInterface_h
#define INCLUDED_vrjugglua_Internal_StringInterface_h

// Internal Includes
// - none

// Library/third-party includes
#include <gadget/Type/StringInterface.h>

// Standard includes
#include <string>

namespace vrjLua {
	namespace Internal {
		class StringInterface {
			public:
				StringInterface(const std::string & device);

				std::string getStringData();

			protected:
				gadget::StringInterface _iface;
		};
	} // end of Internal namespace
} // end of vrjLua namespace

#endif // INCLUDED_vrjugglua_Internal_StringInterface_h
