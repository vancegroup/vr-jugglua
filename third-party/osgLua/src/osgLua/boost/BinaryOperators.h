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
