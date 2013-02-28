/** @file
	@brief Header

	@date 2013

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
	
*/

//           Copyright Iowa State University 2013.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#ifndef INCLUDED_InternalConfig_h_GUID_CBD2CA26_4FBA_479E_AD82_8995BA73B4D9
#define INCLUDED_InternalConfig_h_GUID_CBD2CA26_4FBA_479E_AD82_8995BA73B4D9

#if !defined(OSGLUA_LIBRARY_STATIC) && !defined(OSGLUA_LIBRARY)
// Default to building shared library
#  define OSGLUA_LIBRARY
#endif

#include <osgLua/Config>

#endif // INCLUDED_InternalConfig_h_GUID_CBD2CA26_4FBA_479E_AD82_8995BA73B4D9

