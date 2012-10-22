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
#ifndef INCLUDED_Division_h_GUID_b4eea61a_3c03_4428_a458_6f74aae5a470
#define INCLUDED_Division_h_GUID_b4eea61a_3c03_4428_a458_6f74aae5a470

// Internal Includes
#include "TypePredicates.h"
#include "OperatorBase.h"

// Library/third-party includes
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/and.hpp>

// Standard includes
// - none


namespace osgTraits {
	struct Division;

	namespace Division_Tags {
		using boost::enable_if;
		using boost::mpl::and_;

		struct QuatDivision;
		struct VectorScaling;

		template<typename T1, typename T2, typename = void>
		struct Compute {
			typedef void type;
		};

		template<typename T1, typename T2>
		struct Compute < T1, T2, typename enable_if <
				AreCompatibleQuats<T1, T2> >::type > {
			typedef QuatDivision type;
		};

		template<typename T1, typename T2>
		struct Compute < T1, T2, typename enable_if <
				and_ <
				is_vector<T1>,
				is_scalar<T2> > >::type > {
			typedef VectorScaling type;
		};
	}


	namespace detail {
		template<typename Tag>
		struct Division_impl;

		template<typename T1, typename T2>
		struct Division_Specialization :
				Division_impl<typename Division_Tags::Compute<T1, T2>::type>::template apply<T1, T2>,
		              BinarySpecializedOperator<Division, T1, T2> {
		                  typedef Division_Specialization<T1, T2> type;
		              };

		template<typename Tag>
		struct Division_impl {
			template<typename T1, typename T2>
			struct apply {
			};
		};

		/// Two quats: division
		template<>
		struct Division_impl <Division_Tags::QuatDivision> {
			template<typename T1, typename T2>
			struct apply {
				typedef T1 return_type;
				static return_type performOperation(T1 const& a1, T2 const& a2) {
					return a1 / a2;
				}
			};
		};

		/// Vector and scalar: scaling
		template<>
		struct Division_impl <Division_Tags::VectorScaling> {
			template<typename T1, typename T2>
			struct apply {
				typedef T1 return_type;
				static return_type performOperation(T1 const& a1, T2 const& a2) {
					return a1 / a2;
				}
			};
		};
	} // end of namespace detail

	struct Division : BinaryOperatorBase {
		template<typename T1, typename T2>
		struct apply : detail::Division_Specialization<T1, T2> {
			typedef detail::Division_Specialization<T1, T2> type;
		};
	};

} // end of namespace osgTraits

#endif // INCLUDED_Division_h_GUID_b4eea61a_3c03_4428_a458_6f74aae5a470
