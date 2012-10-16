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
#ifndef INCLUDED_TypePredicates_h_GUID_746880c7_3f70_40c4_99e6_0717ac100770
#define INCLUDED_TypePredicates_h_GUID_746880c7_3f70_40c4_99e6_0717ac100770

// Internal Includes
#include "OsgMathTraits.h"
#include "CompatibleScalar.h"

// Library/third-party includes
#include <boost/mpl/equal.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/and.hpp>

// Standard includes
// - none

namespace osgTraits {
	namespace BinaryPredicates {
		template<typename T1, typename T2>
		struct HaveCompatibleScalar : CompatibleScalar<typename GetValueType<T1>::type, typename GetValueType<T2>::type> {};

		template<typename T1, typename T2>
		struct HaveSameCategory : boost::mpl::equal<typename GetCategory<T1>::type, typename GetCategory<T2>::type>::type {};

		template<typename T1, typename T2>
		struct HaveSameDimension : boost::mpl::equal<typename GetDimension<T1>::type, typename GetDimension<T2>::type>::type {};

		template<typename V, typename M>
		struct CanTransformVecMatrix_impl {
			typedef typename GetDimension<V>::type VecDim;
			typedef typename GetDimension<M>::type MatDim;
			typedef typename boost::mpl::and_ <
			boost::mpl::or_
			< boost::mpl::equal<VecDim, boost::mpl::int_<3> >
			, boost::mpl::equal<VecDim, boost::mpl::int_<4> >
			>
			, boost::mpl::equal<MatDim, boost::mpl::int_<4> >
			>::type type;
		};

		template<typename V, typename M>
		struct CanTransformVecMatrix : CanTransformVecMatrix_impl<V, M>::type {};

		template<typename T1, typename T2>
		struct AreVectorAndMatrix : boost::mpl::and_ <
				is_vector<T1>,
				is_matrix<T2>
				>::type {};

		template<typename T1, T2>
		struct HaveSameCategoryAndDimensionWithCompatibleScalar : boost::mpl::and_<HaveSameCategory<T1, T2>, HaveCompatibleScalar<T1, T2>, HaveSameDimension<T1, T2> >::type {};

	} // end of namespace BinaryPredicates
	namespace UnaryPredicates {

	} // end of namespace UnaryPredicates
} // end of namespace osgTraits
#endif // INCLUDED_TypePredicates_h_GUID_746880c7_3f70_40c4_99e6_0717ac100770
