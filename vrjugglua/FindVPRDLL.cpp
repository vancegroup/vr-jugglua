/**
	@file
	@brief Implementation

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

// Internal Includes
#include "FindVPRDLL.h"

// Library/third-party includes
#include <vpr/vprDefines.h>             // for VPR_OS_Linux

// Standard includes
// - none


#ifdef VPR_OS_Linux

#include <link.h>

extern "C" {
	static int sharedObjectCallback(struct dl_phdr_info *info,
	                                size_t, void *data);
}

int sharedObjectCallback(struct dl_phdr_info *info, size_t, void *data) {
	std::string fn(info->dlpi_name);
	if (fn.find("vpr") != std::string::npos) {
		(*static_cast<std::string*>(data)) = fn;
	}
	return 0;
}

std::string findVPRDLL() {
	std::string vprLibraryPath;
	dl_iterate_phdr(&sharedObjectCallback, &vprLibraryPath);
	return vprLibraryPath;
}

#else

/// other OS
std::string findVPRDLL() {
	return std::string();
}

#endif

