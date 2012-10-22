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
#ifndef INCLUDED_ComputeCategoryTag_h_GUID_49745613_01fb_41d6_a1a0_7a8e3fc30098
#define INCLUDED_ComputeCategoryTag_h_GUID_49745613_01fb_41d6_a1a0_7a8e3fc30098

// Internal Includes
#include "MathTypes.h"
#include "IsScalar.h"
#include "Tags.h"

// Library/third-party includes
#include <boost/utility/enable_if.hpp>


// Standard includes
// - none

namespace osgTraits {
	namespace detail {
		template<typename T, typename = void>
		struct ComputeCategoryTag {
			typedef void type;
		};

		template<typename T>
		struct ComputeCategoryTag<T, typename boost::enable_if<is_matrix<T> >::type> {
			typedef tags::Matrix type;
		};

		template<typename T>
		struct ComputeCategoryTag<T, typename boost::enable_if<is_vector<T> >::type> {
			typedef tags::Vec type;
		};

		template<typename T>
		struct ComputeCategoryTag<T, typename boost::enable_if<is_quat<T> >::type> {
			typedef tags::Quat type;
		};

		template<typename T>
		struct ComputeCategoryTag<T, typename boost::enable_if<is_scalar<T> >::type> {
			typedef tags::Scalar type;
		};

	} // end of namespace detail
} // end of namespace osgTraits

#endif // INCLUDED_ComputeCategoryTag_h_GUID_49745613_01fb_41d6_a1a0_7a8e3fc30098
