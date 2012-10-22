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
#ifndef INCLUDED_UnaryMinus_h_GUID_e5c4ce8f_a7df_4171_b7c0_672766d35673
#define INCLUDED_UnaryMinus_h_GUID_e5c4ce8f_a7df_4171_b7c0_672766d35673

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
	struct UnaryMinus;

	namespace UnaryMinus_Tags {
		using boost::enable_if;
		using boost::mpl::or_;

		struct SimpleUnaryMinus;

		template<typename T1, typename = void>
		struct Compute {
			typedef void type;
		};

		template<typename T1>
		struct Compute < T1, typename enable_if <
				or_ <
				AreCompatibleVectors<T1, T2>,
				AreCompatibleQuats<T1, T2> > >::type > {
			typedef SimpleUnaryMinus type;
		};

	}

	namespace detail {
		template<typename Tag>
		struct UnaryMinus_impl;

		template<typename T1>
		struct UnaryMinus_Specialization :
				UnaryMinus_impl<typename UnaryMinus_Tags::Compute<T1>::type>::template apply<T1>,
		                UnarySpecializedOperator<UnaryMinus, T1> {
		                    typedef UnaryMinus_Specialization<T1> type;
		                };

		template<typename Tag>
		struct UnaryMinus_impl {
			template<typename T1>
			struct apply {
			};
		};

		/// Simple unary minus.
		template<>
		struct UnaryMinus_impl <UnaryMinus_Tags::SimpleUnaryMinus> {
			template<typename T>
			struct apply {
				typedef T return_type;

				static return_type performOperation(T1 const& a) {
					return -a;
				}
			};
		};
	} // end of namespace detail

	struct UnaryMinus : UnaryOperatorBase {
		template<typename T1>
		struct apply : detail::UnaryMinus_Specialization<T1> {
			typedef detail::UnaryMinus_Specialization<T1> type;
		};
	};

} // end of namespace osgTraits


#endif // INCLUDED_UnaryMinus_h_GUID_e5c4ce8f_a7df_4171_b7c0_672766d35673
