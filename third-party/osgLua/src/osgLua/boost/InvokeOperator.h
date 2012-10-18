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
// - none

// Standard includes
// - none

namespace osgTraits {

	template<typename SpecOp>
	struct get_operator_return_type {
		typedef typename SpecOp::return_type type;
	};

	template<typename Op, typename A1, typename A2>
	struct InvokeBinaryOperator {
		typedef typename boost::mpl::apply<Op, A1, A2>::type SpecOp;
		typedef typename get_operator_return_type<SpecOp>::type return_type;
		static return_type invoke(A1 const& a1, A2 const& a2) {
			return SpecOp::performOperation(a1, a2);
		}
	};

	template<typename Op, typename A1, typename A2>
	inline
	typename InvokeBinaryOperator<Op, A1, A2>::return_type
	invokeOperator(A1 const& a1, A2 const& a2) {
		return InvokeBinaryOperator<Op, A1, A2>::invoke(a1, a2);
	};

	template<typename Op, typename A1>
	struct InvokeUnaryOperator {
		typedef typename boost::mpl::apply<Op, A1>::type SpecOp;
		typedef typename get_operator_return_type<SpecOp>::type return_type;
		static return_type invoke(A1 const& a1) {
			return SpecOp::performOperation(a1);
		}
	};

	template<typename Op, typename A1>
	inline
	typename InvokeUnaryOperator<Op, A1>::return_type
	invokeOperator(A1 const& a1) {
		return InvokeUnaryOperator<Op, A1>::invoke(a1);
	};
} // end of namespace osgTraits

#endif // INCLUDED_InvokeOperator_h_GUID_a76515c6_9231_4154_a0cb_168bce258629
