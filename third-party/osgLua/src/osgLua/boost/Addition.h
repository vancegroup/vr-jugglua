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
	struct Addition;

	namespace Addition_Tags {
		using boost::enable_if;
		using boost::mpl::and_;
		using boost::mpl::or_;

		struct Componentwise;

		template<typename T1, typename T2, typename = void>
		struct Compute {
			typedef void type;
		};

		template<typename T1, typename T2>
		struct Compute < T1, T2, typename enable_if <
				or_ <
				are_compatible_vectors<T1, T2>,
				are_compatible_quats<T1, T2> > >::type > {
			typedef Componentwise type;
		};

	}

	namespace detail {
		template<typename Tag>
		struct Addition_impl;

		template<typename T1, typename T2>
		struct Addition_Specialization :
				Addition_impl<typename Addition_Tags::Compute<T1, T2>::type>::template apply<T1, T2>,
		              BinarySpecializedOperator<Addition, T1, T2> {
		                  typedef Addition_Specialization<T1, T2> type;
		              };

		template<typename Tag>
		struct Addition_impl {
			template<typename T1, typename T2>
			struct apply {
			};
		};

		/// Two vectors: subtraction.
		template<>
		struct Addition_impl <Addition_Tags::Componentwise> {

			template<typename T1, typename T2>
			struct apply {
				typedef typename promote_type_with_scalar<T1, typename get_scalar<T2>::type>::type return_type;

				template<typename A, typename B>
				static return_type performOperation(A const& v1, B const& v2) {
					return return_type(v1) + return_type(v2);
				}
			};
		};
	} // end of namespace detail

	struct Addition : BinaryOperatorBase {
		template<typename T1, typename T2>
		struct apply : detail::Addition_Specialization<T1, T2> {
			typedef detail::Addition_Specialization<T1, T2> type;
		};
	};

} // end of namespace osgTraits

#endif // INCLUDED_Addition_h_GUID_effb686e_cc48_46c5_a455_cc60d5929ca9
