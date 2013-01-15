/** @file
	@brief	Turn GMTL matrices into OSG ones.

	@date	2010

	This header is maintained as a part of 'util-headers' - you can always
	find the latest version online at https://github.com/vancegroup/util-headers

	This GUID can help identify the project: d1dbc94e-e863-49cf-bc08-ab4d9f486613

	This copy of the header is from the revision that Git calls
	efa78147fcc5e77a0c724a55bab3b05328ebb6b1

	Commit date: "2012-09-07 15:24:13 -0500"

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

//          Copyright Iowa State University 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#ifndef INCLUDED_gmtlToOsgMatrix_h_GUID_8edbb1ee_508f_43e1_858e_c11b7604eb0f
#define INCLUDED_gmtlToOsgMatrix_h_GUID_8edbb1ee_508f_43e1_858e_c11b7604eb0f

// Internal Includes
// - none

// Library/third-party includes
#include <gmtl/Matrix.h>

#include <osg/Matrix>

// Standard includes
// - none

namespace util {

/// @addtogroup FreeFunctions Free Functions
/// @{

/// Given a GMTL matrix, produce an equivalent OSG matrix
	inline ::osg::Matrix toOsgMatrix(const gmtl::Matrix44f & r) {
		::osg::Matrix temp;
		temp.set(r.getData());
		return temp;
	}

/// @}

} // end of util namespace

#endif // INCLUDED_gmtlToOsgMatrix_h_GUID_8edbb1ee_508f_43e1_858e_c11b7604eb0f

