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
#ifndef INCLUDED_TagMetafunctions_h_GUID_b3581cba_1add_4518_b2ab_3bf71cb90454
#define INCLUDED_TagMetafunctions_h_GUID_b3581cba_1add_4518_b2ab_3bf71cb90454

// Internal Includes
#include "MathTypes.h"
#include "Tags.h"

// Library/third-party includes
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_arithmetic.hpp>

// Standard includes
// - none

namespace osgTraits {
	namespace detail {
		template<typename T>
		struct IsScalar : boost::is_arithmetic<T> {};

		template<typename T, typename = void>
		struct ComputeCategoryTag;

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
		struct ComputeCategoryTag<T, typename boost::enable_if<IsScalar<T> >::type> {
			typedef tags::Scalar type;
		};

		template<typename T, typename = void>
		struct ComputeIsMathTag;

		template<typename T>
		struct ComputeIsMathTag<T, typename boost::enable_if<is_math_type<T> >::type> {
			typedef tags::MathType type;
		};

		template<typename T>
		struct ComputeIsMathTag<T, typename boost::enable_if<IsScalar<T> >::type> {
			typedef tags::Scalar type;
		};

	} // end of namespace detail
} // end of namespace osgTraits

#endif // INCLUDED_TagMetafunctions_h_GUID_b3581cba_1add_4518_b2ab_3bf71cb90454
