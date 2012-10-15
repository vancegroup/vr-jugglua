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
#ifndef INCLUDED_OsgMathTraits_h_GUID_6c3ba294_4496_423b_9e5c_598e2ca2040a
#define INCLUDED_OsgMathTraits_h_GUID_6c3ba294_4496_423b_9e5c_598e2ca2040a

// Internal Includes
#include "MathTypes.h"
#include "Tags.h"


// Library/third-party includes
#include <boost/enable_if.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_arithmetic.hpp>

// Standard includes
// - none

namespace osgTraits {

	template<typename T>
	struct is_math_type : boost::contains<math_types, T>::type {};
	/*
		template<typename T>
		struct IsMathType : boost::mpl::or_<is_matrix<T>, is_vector<T>, is_quat<T> >::type {};
	*/

	template<typename T>
	struct is_scalar : boost::is_arithmetic<T>::type {};

	template<typename T>
	struct is_math_or_scalar : boost::mpl::or_<is_math_type<T>, is_scalar<T> >::type {};

	template<typename T, typename = void>
	struct GetCategory;

	template<typename T>
	struct GetCategory<T, typename boost::enable_if<is_matrix<T> >::type> {
		typedef tags::Matrix type;
	};

	template<typename T>
	struct GetCategory<T, typename boost::enable_if<is_vector<T> >::type> {
		typedef tags::Vec type;
	};

	template<typename T>
	struct GetCategory<T, typename boost::enable_if<is_quat<T> >::type> {
		typedef tags::Quat type;
	};

	template<typename T>
	struct GetCategory<T, typename boost::enable_if<is_scalar<T> >::type> {
		typedef tags::Scalar type;
	};

	template<typename T, typename = void>
	struct GetDimension {
		typedef void type;
	};

	template<typename T>
	struct GetDimension<T, typename boost::enable_if<is_vector<T> >::type> {
		typedef boost::mpl::int_<T::num_components> type;
	};

	template<typename T>
	struct GetDimension<T, typename boost::enable_if<is_matrix<T> >::type> {
		typedef boost::mpl::int_<4> type;
	};

	template<typename T, typename = void>
	struct GetScalar;

	template<typename T>
	struct GetScalar<T, typename boost::enable_if<is_math_type<T> >::type> {
		typedef typename T::value_type type;
	};

	template<typename T>
	struct GetScalar<T, typename boost::enable_if<is_scalar<T> >::type> {
		typedef typename T type;
	};

	template<typename CategoryTag, typename ScalarTag, typename DimensionTag = void>
	struct MathTypeDetail {
		typedef CategoryTag category_tag;
		typedef ScalarTag scalar_tag;
		typedef DimensionTag dimension_tag;
	};

	template<typename T>
	struct GetMathTypeDetail {
		typedef MathTypeDetail<typename GetCategory<T>::type, typename GetScalar<T>::type, typename GetDimension<T>::type> type;
	};

} // end of namespace osgTraits
#endif // INCLUDED_OsgMathTraits_h_GUID_6c3ba294_4496_423b_9e5c_598e2ca2040a
