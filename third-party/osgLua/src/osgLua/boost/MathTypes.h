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
#ifndef INCLUDED_MathTypes_h_GUID_c6ddc37d_59b8_498c_b575_dad4b726de51
#define INCLUDED_MathTypes_h_GUID_c6ddc37d_59b8_498c_b575_dad4b726de51

// Internal Includes
#include "TypeLists.h"
#include "CopyToFlatSequence.h"

// Library/third-party includes
#include <boost/mpl/joint_view.hpp>

// Standard includes
// - none

namespace osgTraits {
	namespace detail {
		using boost::mpl::joint_view;

		struct math_types : copy_to_flat_sequence<joint_view< joint_view<matrix_types, vector_types>, quat_types> >::type {};
	} // end of namespace detail

	using detail::math_types;

} // end of namespace osgTraits

#endif // INCLUDED_MathTypes_h_GUID_c6ddc37d_59b8_498c_b575_dad4b726de51
