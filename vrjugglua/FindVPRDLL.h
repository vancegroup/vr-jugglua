/** @file
	@brief Header

	@date 2011

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

//          Copyright Iowa State University 2011.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#ifndef INCLUDED_FindVPRDLL_h_GUID_745f5a55_6c2b_49ad_9752_9bc57a4d980f
#define INCLUDED_FindVPRDLL_h_GUID_745f5a55_6c2b_49ad_9752_9bc57a4d980f

// Internal Includes
// - none

// Library/third-party includes
// - none

// Standard includes
#include <string>

/// Find the path to the VPR dynamic library, or an empty string if not possible.
/// (Implementation is platform-specific so not available everywhere.)
std::string findVPRDLL();

#endif // INCLUDED_FindVPRDLL_h_GUID_745f5a55_6c2b_49ad_9752_9bc57a4d980f
