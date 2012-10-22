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
#ifndef INCLUDED_Equality_h_GUID_3320559d_0f91_4060_b24e_3fa581f105f1
#define INCLUDED_Equality_h_GUID_3320559d_0f91_4060_b24e_3fa581f105f1

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
	struct Equality;

	template<>
	struct OperatorVerb<Equality> {
		static const char * get() {
			return "compare for equality";
		}
	};

	namespace Equality_Tags {
		using boost::enable_if;
		using boost::mpl::and_;
		using boost::is_same;

		struct MatrixEquality;

		template<typename T1, typename T2, typename = void>
		struct Compute {
			typedef void type;
		};

		template<typename T1, typename T2>
		struct Compute < T1, T2, typename enable_if <
				and_ <
				is_matrix<T1>,
				is_same<T1, T2> > >::type > {
			typedef MatrixEquality type;
		};
	}

	namespace detail {
		template<typename Tag>
		struct Equality_impl;

		template<typename T1, typename T2>
		struct Equality_Specialization :
				Equality_impl<typename Equality_Tags::Compute<T1, T2>::type>::template apply<T1, T2>,
		              BinarySpecializedOperator<Equality, T1, T2> {
		                  typedef Equality_Specialization<T1, T2> type;
		              };

		template<typename Tag>
		struct Equality_impl {
			template<typename T1, typename T2>
			struct apply {
			};
		};

		/// Matrix equality.
		template<>
		struct Equality_impl <Equality_Tags::MatrixEquality> {
			template<typename T1, typename T2>
			struct apply {
				typedef bool return_type;
				static return_type performOperation(T1 const& a1, T2 const& a2) {
					return a1 == a2;
				}
			};
		};
	} // end of namespace detail

	struct Equality : BinaryOperatorBase {
		template<typename T1, typename T2>
		struct apply : detail::Equality_Specialization<T1, T2> {
			typedef detail::Equality_Specialization<T1, T2> type;
		};
	};

} // end of namespace osgTraits

#endif // INCLUDED_Equality_h_GUID_3320559d_0f91_4060_b24e_3fa581f105f1
