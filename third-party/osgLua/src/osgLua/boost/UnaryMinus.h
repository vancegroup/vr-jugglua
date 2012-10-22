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
#include "OperatorBase.h"

// Library/third-party includes
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/and.hpp>

// Standard includes
// - none


namespace osgTraits {
	struct UnaryMinus;

	namespace UnaryMinus_Tags {
		using boost::enable_if;
		using boost::mpl::or_;
		using boost::mpl::and_;

		struct SimpleUnaryMinus;

		template<typename T, typename = void>
		struct Compute {
			typedef void type;
		};

		template<typename T>
		struct Compute < T, typename enable_if <
				and_ <
				or_ <
				is_vector<T>,
				is_quat<T> > ,
				has_floating_point_scalar<T> > >::type > {
			typedef SimpleUnaryMinus type;
		};

	}

	namespace detail {
		template<typename Tag>
		struct UnaryMinus_impl;

		template<typename T>
		struct UnaryMinus_Specialization :
				UnaryMinus_impl<typename UnaryMinus_Tags::Compute<T>::type>::template apply<T>,
		                UnarySpecializedOperator<UnaryMinus, T> {
		                    typedef UnaryMinus_Specialization<T> type;
		                };

		template<typename Tag>
		struct UnaryMinus_impl {
			template<typename T>
			struct apply {
			};
		};

		/// Simple unary minus.
		template<>
		struct UnaryMinus_impl <UnaryMinus_Tags::SimpleUnaryMinus> {
			template<typename T>
			struct apply {
				typedef T return_type;

				static return_type performOperation(T const& a) {
					return -a;
				}
			};
		};
	} // end of namespace detail

	struct UnaryMinus : UnaryOperatorBase {
		template<typename T>
		struct apply : detail::UnaryMinus_Specialization<T> {
			typedef detail::UnaryMinus_Specialization<T> type;
		};
	};

} // end of namespace osgTraits


#endif // INCLUDED_UnaryMinus_h_GUID_e5c4ce8f_a7df_4171_b7c0_672766d35673
