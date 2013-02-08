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
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_base_and_derived.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/less.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/unpack_args.hpp>
#include <boost/utility/enable_if.hpp>

// Standard includes
// - none

namespace osgTraits {
	namespace detail {
		struct UnarySpecializedOperatorBase {};
		struct BinarySpecializedOperatorBase {};
		struct UnimplementedOperationBase {
			typedef void unimplemented_tag;
		};
	} // end of namespace detail

	
	template<typename Operator, typename T1, typename = void>
	struct UnaryOperatorImplementation : UnimplementedOperationBase {};

	template<typename Operator, typename T1, typename T2, typename = void>
	struct BinaryOperatorImplementation : UnimplementedOperationBase {};

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
		typedef boost::mpl::int_<2> operator_arity;
		typedef Op unspecialized_operator_type;
	};
} // end of namespace osgTraits

#endif // INCLUDED_OperatorBase_h_GUID_f6f3382a_be9f_4e4c_a166_43dc8bb15d40
