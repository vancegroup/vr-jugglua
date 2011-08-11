/** @file
	@brief Header defining osgLuaBind verbose message macro

	@date 2011

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
#ifndef INCLUDED_osgLuaBind_verbose_h_GUID_9b0c2044_ffc4_4891_91d4_c6a125617c56
#define INCLUDED_osgLuaBind_verbose_h_GUID_9b0c2044_ffc4_4891_91d4_c6a125617c56

// Internal Includes
// - none

// Library/third-party includes
// - none

// Standard includes
#ifdef BUILD_OSGLUABIND_VERBOSE
#include <ostream>
#define OSGLUABIND_VERBOSE(X) std::cout << "[osgLuaBind] " << X << "\t (at " << __FILE__ << ":" << __LINE__ << ")" << std::endl
#else
#define OSGLUABIND_VERBOSE(X)
#endif

#endif // INCLUDED_osgLuaBind_verbose_h_GUID_9b0c2044_ffc4_4891_91d4_c6a125617c56
