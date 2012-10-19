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
#ifndef INCLUDED_IsMathType_h_GUID_f2b404e2_5034_4d62_9a8d_abc30e11f3cc
#define INCLUDED_IsMathType_h_GUID_f2b404e2_5034_4d62_9a8d_abc30e11f3cc

// Internal Includes
#include "MathTypes.h"

// Library/third-party includes
#include <boost/mpl/or.hpp>

// Standard includes
// - none

namespace osgTraits {
	template<typename T>
	struct is_math_type : boost::mpl::or_ <
			is_matrix<T>,
			is_vector<T>,
			is_quat<T> > {};
} // end of namespace osgTraits

#endif // INCLUDED_IsMathType_h_GUID_f2b404e2_5034_4d62_9a8d_abc30e11f3cc
