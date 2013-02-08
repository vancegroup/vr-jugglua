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
#ifndef INCLUDED_Operators_h_GUID_3d59f8a0_c7d6_48ec_856e_c6bfe11ba582
#define INCLUDED_Operators_h_GUID_3d59f8a0_c7d6_48ec_856e_c6bfe11ba582

// Internal Includes
#include "OperatorBase.h"
#include "UnaryOperators.h"
#include "BinaryOperators.h"
#include "CopyToFlatSequence.h"

// Library/third-party includes
#include <boost/mpl/joint_view.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/or.hpp>

// Standard includes
// - none

namespace osgTraits {
	struct MathOperators : detail::copy_to_flat_sequence < boost::mpl::joint_view<BinaryOperators, UnaryOperators> >::type {};


	namespace operation_detail {
		struct Placeholder;
		using boost::enable_if;
		using boost::mpl::int_;
		using boost::mpl::not_;
		using boost::mpl::equal_to;
		using boost::mpl::at_c;
		using boost::mpl::and_;
		using boost::mpl::apply;
		using boost::is_same;

		using boost::mpl::vector2;
		using boost::mpl::vector3;
		using boost::mpl::unpack_args;
		namespace mpl = boost::mpl;

		template<typename Operator>
		struct get_operator_arity {
			typedef typename Operator::operator_arity type;
		};

		template<typename Operation>
		struct get_sequence : Operation {};

		template<typename Operation>
		struct get_operator {
			typedef typename at_c<Operation, 0>::type type;
		};

		template<typename Operation, int Num>
		struct get_operation_argument_c : at_c < Operation, Num + 1 > {};

		template<typename Operation, int Num>
		struct is_operation_argument_missing : is_same<get_operation_argument_c<Operation, Num>, Placeholder> {};

		template<typename Operation, int Num>
		struct is_operation_argument_supplied : not_<is_operation_argument_missing<Operation, Num> > {};

		template<typename Operator, int N>
		struct is_operator_arity : equal_to<typename get_operator_arity<Operator>::type, int_<N> > {};

		template<typename Operation, int N>
		struct is_operation_arity : is_operator_arity<get_operator<Operation>, N> {};

		template<typename Operation>
		struct get_operation_arity : get_operator_arity<typename get_operator<Operation>::type> {};

		template<typename Operation>
		struct is_operation_unary : is_operation_arity<Operation, 1> {};

		template<typename Operation>
		struct is_operation_binary : is_operation_arity<Operation, 2> {};

		template<typename Sequence>
		struct Operation : Sequence {
			typedef Operation type;
			typedef Sequence sequence_type;
		};
	
		template<typename Operator, typename T1 = Placeholder, typename T2 = Placeholder, typename = void>
		struct construct_operation;

		template<typename Operator, typename T1, typename T2>
		struct construct_operation<Operator, T1, T2, typename enable_if<equal_to<get_operator_arity<Operator>, int_<2> > >::type> {
			typedef Operation<vector3<Operator, T1, T2> > type;
		};

		template<typename Operator, typename T1>
		struct construct_operation<Operator, T1, Placeholder, typename enable_if<equal_to<get_operator_arity<Operator>, int_<1> > >::type> {
			typedef Operation<vector2<Operator, T1> > type;
		};

		template<typename Operation, typename T, typename = void>
		struct add_argtype; /* {
			typedef void type;
		};*/

		template<typename Operation, typename T>
		struct add_argtype < Operation, T, typename enable_if < and_ <
				is_operation_unary<Operation>,
				is_operation_argument_missing<Operation, 0>
				> >::type >{
					typedef typename construct_operation<typename get_operator<Operation>::type, T>::type type;
		};

		template<typename Operation, typename T>
		struct add_argtype < Operation, T, typename enable_if < and_ <
				is_operation_binary<Operation>,
				is_operation_argument_missing<Operation, 0>
				> >::type > {
					typedef typename construct_operation<typename get_operator<Operation>::type, T, typename get_operation_argument_c<Operation, 1>::type>::type type;
		};

		template<typename Operation, typename T>
		struct add_argtype < Operation, T,  typename enable_if < and_ <
				is_operation_binary<Operation>,
				is_operation_argument_supplied<Operation, 0>,
				is_operation_argument_missing<Operation, 1>
				> >::type > {
					typedef typename construct_operation<typename get_operator<Operation>::type, typename get_operation_argument_c<Operation, 0>::type, T>::type type;
		};

		template<typename Operator, typename T, int Arg>
		struct construct_bound_operation;

		template<typename Operator, typename T>
		struct construct_bound_operation<Operator, T, 0> {
			typedef typename construct_operation<Operator, T, Placeholder>::type type;
		};

		template<typename Operator, typename T>
		struct construct_bound_operation<Operator, T, 1> {
			typedef typename construct_operation<Operator, Placeholder, T>::type type;
		};



		template<typename Operation>
		struct UnaryImplementationWrapper
				: UnaryOperatorImplementation <
				typename get_operator<Operation>::type,
				typename get_operation_argument_c<Operation, 0>::type > {};

		template<typename Operation>
		struct BinaryImplementationWrapper
				: BinaryOperatorImplementation <
				typename get_operator<Operation>::type,
				typename get_operation_argument_c<Operation, 0>::type,
				typename get_operation_argument_c<Operation, 1>::type > {};

		template<typename Operation, typename = void>
		struct get_operation_invoker;

		template<typename Operation>
		struct get_operation_invoker<Operation, typename enable_if<	is_operation_unary<Operation> >::type >
				: UnaryImplementationWrapper<typename Operation::sequence_type>::type { };

		template<typename Operation>
		struct get_operation_invoker<Operation, typename enable_if<	is_operation_binary<Operation> >::type>
				: BinaryImplementationWrapper<typename Operation::sequence_type>::type { };
	} // end of namespace operation_detail
	using operation_detail::get_operator_arity;
	using operation_detail::is_operator_arity;
	using operation_detail::construct_operation;
	using operation_detail::construct_bound_operation;
	using operation_detail::add_argtype;
	using operation_detail::get_operation_argument_c;
	using operation_detail::get_operation_invoker;
	using operation_detail::is_operation_unary;
	using operation_detail::is_operation_binary;
	


} // end of namespace osgTraits

#endif // INCLUDED_Operators_h_GUID_3d59f8a0_c7d6_48ec_856e_c6bfe11ba582
