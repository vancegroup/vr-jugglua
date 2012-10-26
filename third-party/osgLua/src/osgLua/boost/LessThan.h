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
#ifndef INCLUDED_LessThan_h_GUID_0d282eed_0a0f_4e86_a94b_a62201d38e7f
#define INCLUDED_LessThan_h_GUID_0d282eed_0a0f_4e86_a94b_a62201d38e7f


// Internal Includes
#include "TypePredicates.h"
#include "OperatorBase.h"

// Library/third-party includes
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/and.hpp>

// Standard includes
// - none

namespace osgTraits {
	struct LessThan;

	template<>
	struct OperatorVerb<LessThan> {
		static const char * get() {
			return "compare for less-than";
		}
	};

	namespace LessThan_Tags {
		using boost::enable_if;
		using boost::mpl::and_;
		using boost::is_same;

		struct MatrixLessThan;

		template<typename T1, typename T2, typename = void>
		struct Compute {
			typedef void type;
		};

		template<typename T1, typename T2>
		struct Compute < T1, T2, typename enable_if <
				and_ <
				is_matrix<T1>,
				is_same<T1, T2> > >::type > {
			typedef MatrixLessThan type;
		};
	}

	namespace detail {
		template<typename Tag>
		struct LessThan_impl;

		template<typename T1, typename T2>
		struct LessThan_Specialization :
				LessThan_impl<typename LessThan_Tags::Compute<T1, T2>::type>::template apply<T1, T2>,
		              BinarySpecializedOperator<LessThan, T1, T2> {};

		template<typename Tag>
		struct LessThan_impl {
			template<typename T1, typename T2>
			struct apply {
			};
		};

		/// Matrix LessThan.
		template<>
		struct LessThan_impl <LessThan_Tags::MatrixLessThan> {
			template<typename T1, typename T2>
			struct apply {
				typedef bool return_type;
				static return_type performOperation(T1 const& a1, T2 const& a2) {
					return a1 == a2;
				}
			};
		};
	} // end of namespace detail

	struct LessThan : BinaryOperatorBase {
		template<typename T1, typename T2>
		struct apply {
			typedef detail::LessThan_Specialization<T1, T2> type;
		};
	};

} // end of namespace osgTraits

#endif // INCLUDED_LessThan_h_GUID_0d282eed_0a0f_4e86_a94b_a62201d38e7f
