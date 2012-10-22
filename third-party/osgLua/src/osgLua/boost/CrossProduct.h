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
#ifndef INCLUDED_CrossProduct_h_GUID_8a1558aa_6ee4_4797_858b_5d19f76dc1cd
#define INCLUDED_CrossProduct_h_GUID_8a1558aa_6ee4_4797_858b_5d19f76dc1cd


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
	struct CrossProduct;

	namespace CrossProduct_Tags {
		using boost::enable_if;
		using boost::mpl::and_;
		using boost::mpl::or_;

		struct VectorCrossProduct;

		template<typename T1, typename T2, typename = void>
		struct Compute {
			typedef void type;
		};

		template<typename T1, typename T2>
		struct Compute < T1, T2, typename enable_if <
				and_ <
				HasDimension3<T1>,
				HasDimension3<T2>,
				is_vector<T1>,
				is_vector<T2>,
				and_ <
				HasFloatingPointScalar<T1>,
				HasFloatingPointScalar<T2> > > >::type > {
			typedef VectorCrossProduct type;
		};

	}

	namespace detail {
		template<typename Tag>
		struct CrossProduct_impl;

		template<typename T1, typename T2>
		struct CrossProduct_Specialization :
				CrossProduct_impl<typename CrossProduct_Tags::Compute<T1, T2>::type>::template apply<T1, T2>,
		                  BinarySpecializedOperator<CrossProduct, T1, T2> {
		                      typedef CrossProduct_Specialization<T1, T2> type;
		                  };

		template<typename Tag>
		struct CrossProduct_impl {
			template<typename T1, typename T2>
			struct apply {
			};
		};

		/// Two vectors: subtraction.
		template<>
		struct CrossProduct_impl <CrossProduct_Tags::VectorCrossProduct> {

			template<typename T1, typename T2>
			struct apply {
				typedef typename PromoteTypeWithScalar<T1, typename GetScalar<T2>::type>::type return_type;

				template<typename A, typename B>
				static return_type performOperation(A const& v1, B const& v2) {
					return return_type(v1) ^ return_type(v2);
				}
			};
		};
	} // end of namespace detail

	struct CrossProduct : BinaryOperatorBase {
		template<typename T1, typename T2>
		struct apply : detail::CrossProduct_Specialization<T1, T2> {
			typedef detail::CrossProduct_Specialization<T1, T2> type;
		};
	};

} // end of namespace osgTraits


#endif // INCLUDED_CrossProduct_h_GUID_8a1558aa_6ee4_4797_858b_5d19f76dc1cd
