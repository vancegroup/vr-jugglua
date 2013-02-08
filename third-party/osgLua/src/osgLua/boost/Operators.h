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
#include "IsOperatorAvailable.h"

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

	namespace detail {
		using boost::mpl::apply;
		using boost::mpl::quote1;
		using boost::mpl::int_;
		using boost::mpl::or_;
		using boost::mpl::protect;

		template<typename OperatorArity>
		struct is_operator_applicable_impl {
			template<typename Operator, typename T>
			struct apply;
		};
		template<typename Operator, typename T, typename = get_operator_arity<Operator>::type>
		struct is_operator_applicable;
		template<typename Operator, typename T>
		struct is_operator_applicable<Operator, T, int_<1> > : is_operator_available<typename boost::mpl::apply<Operator, T>::type> {};

		template<typename Operator, typename T>
		struct is_operator_applicable<Operator, T, int_<2> > : or_ <
				bound_op_has_overloads<protect<typename operator_bind<Operator, T, 1>::type> >,
				bound_op_has_overloads<protect<typename operator_bind<Operator, T, 2>::type> > > {};
	} // end of namespace detail

	using detail::is_operator_applicable;

} // end of namespace osgTraits

#endif // INCLUDED_Operators_h_GUID_3d59f8a0_c7d6_48ec_856e_c6bfe11ba582
