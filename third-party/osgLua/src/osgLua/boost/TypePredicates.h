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
#include "GetCompatibleScalar.h"

// Library/third-party includes
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_floating_point.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/has_xxx.hpp>

// Standard includes
// - none

namespace osgTraits {
	namespace BinaryPredicates {
		namespace detail {
			BOOST_MPL_HAS_XXX_TRAIT_DEF(type);
		} // end of namepsace detail

		template<typename T1, typename T2, typename = void>
		struct HaveCompatibleScalar {
			typedef boost::mpl::false_ type;
		};

		template<typename T1, typename T2>
		struct HaveCompatibleScalar < T1, T2,
				typename boost::enable_if<detail::has_type<GetCompatibleScalar<typename GetScalar<T1>::type, typename GetScalar<T2>::type> > >::type >
				: boost::mpl::true_ {};

		template<typename T1, typename T2>
		struct HaveSameCategory : boost::is_same<typename GetCategory<T1>::type, typename GetCategory<T2>::type>::type {};

		template<typename T1, typename T2>
		struct HaveSameDimension : boost::is_same<typename GetDimension<T1>::type, typename GetDimension<T2>::type>::type {};

		namespace detail {
			template<typename V, typename M>
			struct CanTransformVecMatrix_impl {
				typedef typename GetDimension<V>::type VecDim;
				typedef typename GetDimension<M>::type MatDim;
				typedef typename boost::mpl::and_ <
				boost::mpl::or_
				< boost::is_same<VecDim, boost::mpl::int_<3> >
				, boost::is_same<VecDim, boost::mpl::int_<4> >
				>
				, boost::is_same<MatDim, boost::mpl::int_<4> >
				> type;
			};
		} // end of namepsace detail

		template<typename V, typename M>
		struct CanTransformVecMatrix : detail::CanTransformVecMatrix_impl<V, M>::type {};

		template<typename T1, typename T2>
		struct AreVectorAndMatrix : boost::mpl::and_ <
				typename is_vector<T1>::type,
				typename is_matrix<T2>::type
				>::type {};

		template<typename T1, typename T2>
		struct HaveSameCategoryAndDimensionWithCompatibleScalar : boost::mpl::and_ <
				HaveSameCategory<T1, T2>,
				HaveCompatibleScalar<T1, T2>,
				HaveSameDimension<T1, T2>
				>::type {};

	} // end of namespace BinaryPredicates
	namespace UnaryPredicates {
		template<typename T>
		struct HasFloatingPointScalar : boost::is_floating_point<typename GetScalar<T>::type> {};
	} // end of namespace UnaryPredicates
} // end of namespace osgTraits
#endif // INCLUDED_TypePredicates_h_GUID_746880c7_3f70_40c4_99e6_0717ac100770
