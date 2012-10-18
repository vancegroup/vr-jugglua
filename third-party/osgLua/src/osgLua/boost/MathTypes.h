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

// Standard includes
// - none


namespace osgTraits {
	using boost::mpl::placeholders::_;
	typedef boost::mpl::joint_view< boost::mpl::joint_view<matrix_types, vector_types>::type, quat_types>::type math_types;
	//typedef boost::mpl::insert_range< boost::mpl::insert_range< matrix_types, vector_types>, quat_types> math_types;
	/*
		typedef boost::mpl::or_<
			boost::mpl::contains<matrix_types, _>,
			boost::mpl::contains<vector_types, _>,
			boost::mpl::contains<quat_types, _> > is_math_type;
		*/

	template<typename T>
	struct is_math_type : boost::mpl::or_ <
			boost::mpl::has_key<matrix_types, T>,
			boost::mpl::has_key<vector_types, T>,
			boost::mpl::has_key<quat_types, T> > {

	};
	/*
	template<typename T>
	struct is_math_type : boost::mpl::or_<
	boost::mpl::contains<matrix_types, T>,

	::type {};
	*/
	typedef boost::mpl::list2<double, float> arithmetic_types;
	typedef boost::mpl::joint_view<math_types, arithmetic_types> math_and_arithmetic_types;
} // end of namespace osgTraits

#endif // INCLUDED_MathTypes_h_GUID_c6ddc37d_59b8_498c_b575_dad4b726de51
