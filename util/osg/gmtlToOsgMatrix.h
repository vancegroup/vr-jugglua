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
#ifndef _util_osg_gmtlToOsgMatrix_h_
#define _util_osg_gmtlToOsgMatrix_h_

// Internal Includes
// - none

// Library/third-party includes
#include <gmtl/Matrix.h>

#include <osg/Matrix>

// Standard includes
// - none

namespace util {

namespace osg {

inline ::osg::Matrix toOsgMatrix(const gmtl::Matrix44f & r) {
	::osg::Matrix temp;
	temp.set(r.getData());
	return temp;
}

} // end of osg namespace

} // end of util namespace

#endif // _util_osg_gmtlToOsgMatrix_h_
