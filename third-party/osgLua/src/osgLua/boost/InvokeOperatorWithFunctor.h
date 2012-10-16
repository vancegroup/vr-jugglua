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
#ifndef INCLUDED_InvokeOperatorWithFunctor_h_GUID_2463847e_dc59_48f4_822f_50644e90cf73
#define INCLUDED_InvokeOperatorWithFunctor_h_GUID_2463847e_dc59_48f4_822f_50644e90cf73

// Internal Includes
#include "Tags.h"

// Library/third-party includes
// - none

// Standard includes
// - none

namespace osgTraits {
	template<typename Tag> struct InvokeOperator_impl;

	template<typename WrappedOp>
	struct InvokeOperator : InvokeOperator_impl<typename WrappedOp::operator_tag>::template apply<WrappedOp> {};


	template<typename Tag>
	struct InvokeOperator_impl {
		template<typename WrappedOp>
		struct apply;
	};

	template<typename F>
} // end of namespace osgTraits

#endif // INCLUDED_InvokeOperatorWithFunctor_h_GUID_2463847e_dc59_48f4_822f_50644e90cf73
