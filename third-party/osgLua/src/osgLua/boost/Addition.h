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
#ifndef INCLUDED_Addition_h_GUID_effb686e_cc48_46c5_a455_cc60d5929ca9
#define INCLUDED_Addition_h_GUID_effb686e_cc48_46c5_a455_cc60d5929ca9

// Internal Includes
#include "TypePredicates.h"
#include "PromoteTypeWithScalar.h"
#include "OperatorBase.h"

// Library/third-party includes
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>

// Standard includes
// - none


namespace osgTraits {
	struct Addition : BinaryOperator<Addition> {};

	template<>
	struct OperatorVerb<Addition> {
		static const char * get() {
			return "add";
		}
	};

	template<typename Ret>
	struct ComponentwiseAddition {
		typedef Ret return_type;
		template<typename A, typename B>
		static return_type performOperation(A const& v1, B const& v2) {
			return return_type(v1) + return_type(v2);
		}
	};
	template<typename T1, typename T2>
	struct BinaryOperatorImplementation < Addition, T1, T2, typename boost::enable_if <
			boost::mpl::or_ <
			are_compatible_vectors<T1, T2>,
			are_compatible_quats<T1, T2> > >::type >  {
		typedef typename promote_type_with_scalar<T1, typename get_scalar<T2>::type>::type return_type;
		typedef ComponentwiseAddition<return_type> apply;
	};

} // end of namespace osgTraits

#endif // INCLUDED_Addition_h_GUID_effb686e_cc48_46c5_a455_cc60d5929ca9
