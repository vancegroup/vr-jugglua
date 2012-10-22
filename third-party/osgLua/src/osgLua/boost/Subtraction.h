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
#ifndef INCLUDED_Subtraction_h_GUID_fec3e459_c1e7_441b_9a1b_bc11a65a8336
#define INCLUDED_Subtraction_h_GUID_fec3e459_c1e7_441b_9a1b_bc11a65a8336

// Internal Includes
#include "TypePredicates.h"
#include "PromoteTypeWithScalar.h"
#include "OperatorBase.h"

// Library/third-party includes
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/or.hpp>

// Standard includes
// - none


namespace osgTraits {
	struct Subtraction;

	namespace Subtraction_Tags {
		using namespace ::osgTraits::BinaryPredicates;
		using boost::enable_if;
		using boost::mpl::or_;

		struct Componentwise;

		template<typename T1, typename T2, typename = void>
		struct Compute {
			typedef void type;
		};

		template<typename T1, typename T2>
		struct Compute < T1, T2, typename enable_if <
				or_ <
				AreCompatibleVectors<T1, T2>,
				AreCompatibleQuats<T1, T2> > >::type > {
			typedef Componentwise type;
		};

	}

	namespace detail {
		template<typename Tag>
		struct Subtraction_impl;

		template<typename T1, typename T2>
		struct Subtraction_Specialization :
				Subtraction_impl<typename Subtraction_Tags::Compute<T1, T2>::type>::template apply<T1, T2>,
		                 BinarySpecializedOperator<Subtraction, T1, T2> {
		                     typedef Subtraction_Specialization<T1, T2> type;
		                 };

		template<typename Tag>
		struct Subtraction_impl {
			template<typename T1, typename T2>
			struct apply {
			};
		};

		/// Two vectors: subtraction.
		template<>
		struct Subtraction_impl <Subtraction_Tags::Componentwise> {
			template<typename T1, typename T2>
			struct apply {
				typedef typename PromoteTypeWithScalar<T1, typename GetScalar<T2>::type>::type return_type;

				template<typename A, typename B>
				static return_type performOperation(A const& v1, B const& v2) {
					return return_type(v1) - return_type(v2);
				}
			};
		};
	} // end of namespace detail

	struct Subtraction : BinaryOperatorBase {
		template<typename T1, typename T2>
		struct apply : detail::Subtraction_Specialization<T1, T2> {
			typedef detail::Subtraction_Specialization<T1, T2> type;
		};
	};

} // end of namespace osgTraits

#endif // INCLUDED_Subtraction_h_GUID_fec3e459_c1e7_441b_9a1b_bc11a65a8336
