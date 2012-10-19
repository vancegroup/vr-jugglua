/** @file
	@brief Header

	@date 2012

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

//          Copyright Iowa State University 2012.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#ifndef INCLUDED_IsMathOrScalar_h_GUID_7c97973c_b48c_44a7_bb14_c72c33d80e6f
#define INCLUDED_IsMathOrScalar_h_GUID_7c97973c_b48c_44a7_bb14_c72c33d80e6f

// Internal Includes
#include "MathTypes.h"
#include "IsScalar.h"

// Library/third-party includes
#include <boost/mpl/or.hpp>

// Standard includes
// - none

namespace osgTraits {

	template<typename T>
	struct is_math_or_scalar : boost::mpl::or_<is_math_type<T>, is_scalar<T> >::type {};

} // end of namespace osgTraits

#endif // INCLUDED_IsMathOrScalar_h_GUID_7c97973c_b48c_44a7_bb14_c72c33d80e6f
