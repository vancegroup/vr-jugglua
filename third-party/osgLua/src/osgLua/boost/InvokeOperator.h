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
#ifndef INCLUDED_InvokeOperator_h_GUID_a76515c6_9231_4154_a0cb_168bce258629
#define INCLUDED_InvokeOperator_h_GUID_a76515c6_9231_4154_a0cb_168bce258629

// Internal Includes
#include "OperatorBase.h"

// Library/third-party includes
#include <boost/mpl/apply.hpp>

// Standard includes
// - none

namespace osgTraits {

#if 0
	template<typename SpecOp>
	struct get_operator_return_type {
		typedef typename SpecOp::return_type type;
	};

	namespace detail {
		/// Generic binary operator invoker, for an unspecialized operator.
		template<typename Op, typename A1 = void, typename A2 = void, typename = typename is_unspecialized_operator<Op>::type >
		struct InvokeBinaryOperator {
			typedef typename boost::mpl::apply<Op, A1, A2>::type SpecOp;
			typedef typename get_operator_return_type<SpecOp>::type return_type;
			static return_type invoke(A1 const& a1, A2 const& a2) {
				return SpecOp::performOperation(a1, a2);
			}
		};

		/// Template specialization to invoke already-specialized operators.
		template<typename Op, typename A1, typename A2>
		struct InvokeBinaryOperator<Op, A1, A2, boost::mpl::false_> {
			typedef Op SpecOp;
			typedef typename get_operator_return_type<SpecOp>::type return_type;
			static return_type invoke(A1 const& a1, A2 const& a2) {
				return SpecOp::performOperation(a1, a2);
			}
		};

		/// Generic unary operator invoker, for an unspecialized operator
		template<typename Op, typename A, typename = typename is_unspecialized_operator<Op>::type >
		struct InvokeUnaryOperator {
			typedef typename boost::mpl::apply<Op, A>::type SpecOp;
			typedef typename get_operator_return_type<SpecOp>::type return_type;
			static return_type invoke(A const& a) {
				return SpecOp::performOperation(a);
			}
		};

		/// Template specialization to invoke already-specialized operators.
		template<typename Op, typename A>
		struct InvokeUnaryOperator<Op, A, boost::mpl::false_> {
			typedef Op SpecOp;
			typedef typename get_operator_return_type<SpecOp>::type return_type;
			static return_type invoke(A const& a) {
				return SpecOp::performOperation(a);
			}
		};
	} // end of namespace detail

	/// Simple function to invoke a binary operator providing type-deduction
	template<typename Op, typename A1, typename A2>
	inline
	typename detail::InvokeBinaryOperator<Op, A1, A2>::return_type
	invokeOperator(A1 const& a1, A2 const& a2) {
		return detail::InvokeBinaryOperator<Op, A1, A2>::invoke(a1, a2);
	};


	/// Simple function to invoke a unary operator providing type-deduction
	template<typename Op, typename A>
	inline
	typename detail::InvokeUnaryOperator<Op, A>::return_type
	invokeOperator(A const& a) {
		return detail::InvokeUnaryOperator<Op, A>::invoke(a);
	};
#endif
	template<typename Operation>
	typename get_operation_invoker<Operation>::return_type invokeOperation(typename get_operation_argument_c<Operation, 0>::type const& a) {
		typedef typename boost::mpl::apply<get_operation_invoker<Operation> >::type Invoker;
		return invoker::performOperation(a);
	}

	template<typename Operation>
	typename get_operation_invoker<Operation>::return_type invokeOperation(typename get_operation_argument_c<Operation, 0>::type const& a, typename get_operation_argument_c<Operation, 1>::type const& b) {
		typedef typename boost::mpl::apply<get_operation_invoker<Operation> >::type Invoker;
		return invoker::performOperation(a, b);
	}

} // end of namespace osgTraits

#endif // INCLUDED_InvokeOperator_h_GUID_a76515c6_9231_4154_a0cb_168bce258629
