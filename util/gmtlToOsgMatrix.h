/** @file	gmtlToOsgMatrix.h
	@brief	Turn GMTL matrices into OSG ones.

	@date	2010

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

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

