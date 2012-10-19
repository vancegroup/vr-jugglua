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

// Library/third-party includes
#include <boost/mpl/joint_view.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/front_inserter.hpp>

// Standard includes
// - none


namespace osgTraits {
	using boost::mpl::placeholders::_;
	typedef boost::mpl::joint_view< boost::mpl::joint_view<matrix_types, vector_types>::type, quat_types>::type math_joint_types;
	typedef boost::mpl::copy<math_joint_types, boost::mpl::front_inserter<boost::mpl::list0<> > >::type math_types;

	template<typename T>
	struct is_math_type : boost::mpl::or_ <
			is_matrix<T>,
			is_vector<T>,
			is_quat<T> > {

	};

	typedef boost::mpl::list2<double, float> arithmetic_types;
	typedef boost::mpl::copy < boost::mpl::joint_view<math_types, arithmetic_types>,
	        boost::mpl::front_inserter<boost::mpl::list0<> > >::type math_and_arithmetic_types;
} // end of namespace osgTraits

#endif // INCLUDED_MathTypes_h_GUID_c6ddc37d_59b8_498c_b575_dad4b726de51
