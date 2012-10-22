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
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/lambda.hpp>

// Standard includes
// - none

namespace osgTraits {
	namespace UnaryPredicates {
		using boost::is_same;
		using boost::mpl::equal_to;
		using boost::mpl::int_;
		using boost::mpl::and_;
		using boost::mpl::or_;
		using boost::mpl::_;
		using boost::mpl::_1;
		using boost::mpl::_2;
		using boost::mpl::apply;


		template<typename T>
		struct has_floating_point_scalar : boost::is_floating_point<typename get_scalar<T>::type> {};

		typedef boost::mpl::lambda<equal_to<get_dimension<_1>, _2 > >::type HasDimensionLambda;

		template<typename T, int Dim>
		struct has_dimension : apply<HasDimensionLambda, T, int_<Dim> >::type {};


		template<typename V>
		struct is_transformable_vector : and_ <
				is_vector<V>,
				or_
				< has_dimension<V, 3>
				, has_dimension<V, 4>
				> >::type {};

	} // end of namespace UnaryPredicates


	using UnaryPredicates::has_dimension;
	using UnaryPredicates::is_transformable_vector;
	using UnaryPredicates::has_floating_point_scalar;

	namespace BinaryPredicates {
		using boost::enable_if;
		using boost::is_same;
		using boost::mpl::equal_to;
		using boost::mpl::true_;
		using boost::mpl::false_;
		using boost::mpl::and_;
		using boost::mpl::or_;

		namespace detail {
			BOOST_MPL_HAS_XXX_TRAIT_DEF(type);
		} // end of namespace detail

		template<typename T1, typename T2, typename = void>
		struct have_compatible_scalar {
			typedef boost::mpl::false_ type;
		};

		template<typename T1, typename T2>
		struct have_compatible_scalar < T1, T2,
				typename enable_if<detail::has_type<get_compatible_scalar<typename get_scalar<T1>::type, typename get_scalar<T2>::type> > >::type >
				: true_ {};

		template<typename T1, typename T2>
		struct have_same_category : boost::is_same<typename get_category<T1>::type, typename get_category<T2>::type>::type {};

		template<typename T1, typename T2>
		struct have_same_dimension : boost::is_same<typename get_dimension<T1>::type, typename get_dimension<T2>::type>::type {};

		namespace detail {
			template<typename V, typename M>
			struct can_transform_vec_matrix_impl {
				typedef typename get_dimension<V>::type VecDim;
				typedef typename get_dimension<M>::type MatDim;
				typedef typename boost::mpl::and_ <
				boost::mpl::or_
				< has_dimension<V, 3>
				, has_dimension<V, 4>
				>
				, has_dimension<M, 4>
				> type;
			};
		} // end of namespace detail

		template<typename V, typename M>
		struct can_transform_vec_matrix : detail::can_transform_vec_matrix_impl<V, M>::type {};

		template<typename T1, typename T2>
		struct are_vec_and_matrix : and_ <
				is_vector<T1>,
				is_matrix<T2>
				>::type {};

		template<typename T1, typename T2>
		struct have_same_cat_and_dim_with_compat_scalar : and_ <
				have_same_category<T1, T2>,
				have_compatible_scalar<T1, T2>,
				have_same_dimension<T1, T2>
				>::type {};

		template<typename T1, typename T2>
		struct are_compatible_vectors : and_ <
				is_vector<T1>,
				have_same_cat_and_dim_with_compat_scalar<T1, T2> >::type {};

		template<typename T1, typename T2>
		struct are_compatible_quats : and_ <
				is_quat<T1>,
				have_same_cat_and_dim_with_compat_scalar<T1, T2> >::type {};

		template<typename T1, typename T2>
		struct are_compatible_matrices : and_ <
				is_matrix<T1>,
				have_same_cat_and_dim_with_compat_scalar<T1, T2> >::type {};
	} // end of namespace BinaryPredicates


	using BinaryPredicates::have_compatible_scalar;
	using BinaryPredicates::have_same_category;
	using BinaryPredicates::have_same_dimension;
	using BinaryPredicates::can_transform_vec_matrix;
	using BinaryPredicates::are_vec_and_matrix;
	using BinaryPredicates::are_compatible_vectors;
	using BinaryPredicates::are_compatible_quats;
	using BinaryPredicates::are_compatible_matrices;

} // end of namespace osgTraits
#endif // INCLUDED_TypePredicates_h_GUID_746880c7_3f70_40c4_99e6_0717ac100770
