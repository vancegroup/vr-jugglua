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
#ifndef INCLUDED_IsOperatorAvailable_h_GUID_ea6e8ae3_7a30_4c88_b99e_5f12b40ee59b
#define INCLUDED_IsOperatorAvailable_h_GUID_ea6e8ae3_7a30_4c88_b99e_5f12b40ee59b

// Internal Includes
#include "Operators.h"

// Library/third-party includes
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_base_and_derived.hpp>

// Standard includes
// - none

namespace osgTraits {
	template<typename Operation>
	struct is_operation_available {
		typedef boost::mpl::not_<boost::is_base_and_derived<detail::UnimplementedOperationBase, get_operation_invoker<Operation> > > type;	
	};
	
	namespace detail {
		namespace mpl = boost::mpl;
		typedef mpl::back_inserter< mpl::list0<> > inserter_type;

		template<typename Operation, typename T>
		struct is_bound_operation_available : is_operation_available<typename add_argtype<Operation, T>::type > {};

		template<typename Operation>
		struct get_valid_other_arg_types {
			typedef typename
			mpl::copy_if <
			other_argument_types,
			is_bound_operation_available<Operation, mpl::_>,
			inserter_type
			>::type type;
		};

	} // end of namespace detail
	using detail::get_valid_other_arg_types;

	namespace detail {
		using boost::mpl::apply;
		using boost::mpl::int_;
		using boost::mpl::or_;
		using boost::mpl::not_;
		using boost::mpl::empty;
		using boost::enable_if;

		template<typename Operator, typename T, typename = void>
		struct is_operator_applicable;
		template<typename Operator, typename T>
		struct is_operator_applicable<Operator, T, typename enable_if<is_operator_arity<Operator, 1> >::type > : is_operation_available<typename construct_operation<Operator, T>::type> {};

		template<typename Operation>
		struct bound_operation_has_implementations {
			typedef typename not_<empty<typename get_valid_other_arg_types<Operation>::type > >::type type;
		};

		template<typename Operator, typename T>
		struct is_operator_applicable<Operator, T, typename enable_if<is_operator_arity<Operator, 2> >::type > : or_ <
				bound_operation_has_implementations<construct_bound_operation<Operator, T, 0> >,
				bound_operation_has_implementations<construct_bound_operation<Operator, T, 1> > > {};
	} // end of namespace detail

	using detail::is_operator_applicable;
	
} // end of namespace osgTraits

#endif // INCLUDED_IsOperatorAvailable_h_GUID_ea6e8ae3_7a30_4c88_b99e_5f12b40ee59b
