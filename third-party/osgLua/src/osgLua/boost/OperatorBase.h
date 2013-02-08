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
#ifndef INCLUDED_OperatorBase_h_GUID_f6f3382a_be9f_4e4c_a166_43dc8bb15d40
#define INCLUDED_OperatorBase_h_GUID_f6f3382a_be9f_4e4c_a166_43dc8bb15d40

// Internal Includes
// - none

// Library/third-party includes
#include <boost/mpl/int.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_base_and_derived.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/less.hpp>

// Standard includes
// - none

namespace osgTraits {
	namespace detail {
		struct UnarySpecializedOperatorBase {};
		struct BinarySpecializedOperatorBase {};
	} // end of namespace detail

	template<typename Operator, typename T>
	struct UnarySpecializedOperator : detail::UnarySpecializedOperatorBase {
		typedef Operator unspecialized_operator_type;
		typedef boost::mpl::int_<1> operator_arity;
		typedef boost::mpl::vector1<T> argument_types;

		BOOST_MPL_ASSERT((boost::mpl::equal<typename Operator::operator_arity, operator_arity>));
		BOOST_MPL_ASSERT((boost::mpl::equal<typename boost::mpl::size<argument_types>::type, operator_arity>));
	};

	template<typename Operator, typename T1, typename T2>
	struct BinarySpecializedOperator : detail::BinarySpecializedOperatorBase {
		typedef Operator unspecialized_operator_type;
		typedef boost::mpl::int_<2> operator_arity;
		typedef boost::mpl::vector2<T1, T2> argument_types;

		BOOST_MPL_ASSERT((boost::mpl::equal<typename Operator::operator_arity, operator_arity>));
		BOOST_MPL_ASSERT((boost::mpl::equal<typename boost::mpl::size<argument_types>::type, operator_arity>));
	};

	template<typename Operator, typename T1, typename = void>
	struct UnaryOperatorImplementation {
		typedef void unimplemented_tag;
	};

	template<typename Operator, typename T1, typename T2, typename = void>
	struct BinaryOperatorImplementation {
		typedef void unimplemented_tag;
	};

	template<typename Operator>
	struct OperatorVerb;

	struct UnaryOperatorBase {
		typedef boost::mpl::int_<1> operator_arity;
	};

	struct BinaryOperatorBase {
		typedef boost::mpl::int_<2> operator_arity;
	};

	template<typename Op>
	struct BinaryOperator : BinaryOperatorBase {
		typedef Op unspecialized_operator_type;
		template<typename T1, typename T2>
		struct apply {
			typedef BinaryOperatorImplementation<Op, T1, T2> type;
		};
	};


	template<typename T>
	struct is_unspecialized_operator :
			boost::mpl::or_<boost::is_base_and_derived<UnaryOperatorBase, T>, boost::is_base_and_derived<BinaryOperatorBase, T> > {};

	template<typename T>
	struct is_specialized_operator :
			boost::mpl::or_<boost::is_base_and_derived<detail::UnarySpecializedOperatorBase, T>, boost::is_base_and_derived<detail::BinarySpecializedOperatorBase, T> > {};

	template<typename T>
	struct is_operator : boost::mpl::or_<is_unspecialized_operator<T>, is_specialized_operator<T> > {};

	template<typename T>
	struct get_operator_arity {
		typedef typename T::operator_arity type;
	};

	template<typename T, typename Index>
	struct get_operator_argument : boost::mpl::at<typename T::argument_types, Index> {
		BOOST_MPL_ASSERT((boost::mpl::less<Index, typename get_operator_arity<T>::type >));
		BOOST_MPL_ASSERT((is_specialized_operator<T>));
	};

	template<typename T, int index>
	struct get_operator_argument_c : get_operator_argument<T, boost::mpl::int_<index> > {};

} // end of namespace osgTraits

#endif // INCLUDED_OperatorBase_h_GUID_f6f3382a_be9f_4e4c_a166_43dc8bb15d40
