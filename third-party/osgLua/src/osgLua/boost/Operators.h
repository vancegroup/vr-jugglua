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
#include "BinaryOperators.h"

// Library/third-party includes
#include <boost/mpl/bool.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/lambda.hpp>

// Standard includes
// - none


namespace osgTraits {
	typedef BinaryOperators MathOperators;

	namespace detail {
		template<typename OperatorArity>
		struct IsOperatorApplicable_impl {
			template<typename Operator, typename T>
			struct apply;
		};
		using boost::mpl::lambda;
		using boost::mpl::_;
		using boost::mpl::_1;
		using boost::mpl::_2;
		using boost::mpl::or_;
		using boost::mpl::apply;
		template<typename Operator, typename T>
		struct IsOperatorApplicable : IsOperatorApplicable_impl<typename apply<boost::mpl::quote1<get_operator_arity>, Operator>::type>::template apply<Operator, T> {};

		template<>
		struct IsOperatorApplicable_impl<boost::mpl::int_<1> > {
			template<typename Operator, typename T>
			struct apply {
				typedef boost::mpl::false_ type;
			};
		};

		template<>
		struct IsOperatorApplicable_impl<boost::mpl::int_<2> > {
			template<typename Operator, typename T>
			struct apply : boost::mpl::or_ <
					BoundOpHasOverloads<boost::mpl::protect<typename OperatorBindFirst<Operator, T>::type> >,
					BoundOpHasOverloads<boost::mpl::protect<typename OperatorBindSecond<Operator, T>::type > > >  {};
		};
		/*
		template<typename T>
		struct get_applicable_operators {
			typedef boost::mpl::filter_view<MathOperators, is_bound_operator_available<boost::mpl::protect<BoundOp>, _> > type;
		};
		*/
	}

	using detail::IsOperatorApplicable;



} // end of namespace osgTraits

#endif // INCLUDED_Operators_h_GUID_3d59f8a0_c7d6_48ec_856e_c6bfe11ba582
