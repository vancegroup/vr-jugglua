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
#ifndef INCLUDED_BinaryOperators_h_GUID_9d5a8223_67c4_4299_99ef_30fe8607bab4
#define INCLUDED_BinaryOperators_h_GUID_9d5a8223_67c4_4299_99ef_30fe8607bab4

// Internal Includes
#include "MathAndArithmeticTypes.h"
#include "IsOperatorAvailable.h"

#include "Addition.h"
#include "Subtraction.h"
#include "Multiplication.h"
#include "Pow.h"
#include "Division.h"
#include "Equality.h"
#include "LessThan.h"

// Library/third-party includes
#include <boost/mpl/bind.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/copy_if.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/protect.hpp>
#include <boost/mpl/empty.hpp>

// Standard includes
// - none

namespace osgTraits {
	typedef math_and_arithmetic_types other_argument_types;

	//typedef boost::mpl::list7<Addition, Subtraction, Multiplication, Pow, Division, Equality, LessThan> BinaryOperators;
	typedef boost::mpl::list1<Addition> BinaryOperators;

	/*

		struct UnspecifiedArgument;
		template<typename Op, typename T1 = UnspecifiedArgument, typename T2 = UnspecifiedArgument>
		struct bound_operator {
			typedef Op operation_type;
			typedef T1 arg_type_1;
			typedef T2 arg_type_2;

			typedef bound_operator<Op, T1, T2> type;
		};

		template<typename BoundOperator, typename NewArg>
		struct bind_other_arg;

		template<typename Op, typename PrevArg, typename NewArg>
		struct bind_other_arg<bound_operator<Op, PrevArg, UnspecifiedArgument>, NewArg> {
			typedef bound_operator<Op, PrevArg, NewArg> type;
		};

		template<typename Op, typename PrevArg, typename NewArg>
		struct bind_other_arg<bound_operator<Op, UnspecifiedArgument, PrevArg>, NewArg> {
			typedef bound_operator<Op, NewArg, PrevArg> type;
		};

		template<typename Op, typename T, int Arg>
		struct create_partial_operator;

		template<typename Op, typename T>
		struct create_partial_operator<Op, T, 1> {
			typedef bound_operator<Op, T, UnspecifiedArgument> type;
		};

		template<typename Op, typename T>
		struct create_partial_operator<Op, T, 2> {
			typedef bound_operator<Op, UnspecifiedArgument, T> type;
		};

		template<typename BoundOperator>
		struct specialize_operator {
			typedef typename BoundOperator::operation_type:: template apply< typename BoundOperator::arg_type_1, typename BoundOperator::arg_type_2>::type type;
		};
	*/
	template<typename Op, typename T, int Arg>
	struct operator_bind;

	template<typename Op, typename T>
	struct operator_bind<Op, T, 1> {
		typedef boost::mpl::bind2<Op, T, boost::mpl::_> type;
	};

	template<typename Op, typename T>
	struct operator_bind<Op, T, 2> {
		typedef boost::mpl::bind2<Op, boost::mpl::_, T> type;
	};

	namespace detail {
		namespace mpl = boost::mpl;
		template<typename BoundOp, typename T>
		struct is_bound_operator_available : is_operator_available<mpl::apply<BoundOp, T> > {};


		template<typename BoundOp>
		struct get_valid_other_arg_types {
			typedef typename
			mpl::copy_if <
			other_argument_types,
			is_bound_operator_available<mpl::protect<BoundOp>, mpl::_>,
			mpl::back_inserter< mpl::list0<> >
			>::type type;
		};
		template<typename BoundOp>
		struct bound_op_has_overloads {
			typedef typename mpl::not_<mpl::empty<typename get_valid_other_arg_types<BoundOp>::type > >::type type;
		};
	} // end of namespace detail
	using detail::get_valid_other_arg_types;
	using detail::bound_op_has_overloads;

} // end of namespace osgTraits
#endif // INCLUDED_BinaryOperators_h_GUID_9d5a8223_67c4_4299_99ef_30fe8607bab4
