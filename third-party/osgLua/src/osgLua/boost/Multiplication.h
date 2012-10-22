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
#ifndef INCLUDED_Multiplication_h_GUID_d7c2cc4f_5b7d_4891_bc1d_6b3a21a9b3c9
#define INCLUDED_Multiplication_h_GUID_d7c2cc4f_5b7d_4891_bc1d_6b3a21a9b3c9

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
	struct Multiplication;

	template<>
	struct OperatorVerb<Multiplication> {
		static const char * get() {
			return "multiply/dot";
		}
	};

	namespace MultiplicationTags {
		using namespace ::osgTraits::BinaryPredicates;
		using boost::enable_if;
		using boost::mpl::and_;
		using boost::mpl::or_;

		struct TransformCompose;
		struct VectorDotProduct;
		struct VectorTimesMatrix;
		struct TransformTimesVector;

		struct VectorScalar;
		struct ScalarVector;

		template<typename T1, typename T2, typename = void>
		struct Compute {
			typedef void type;
		};

		template<typename T1, typename T2>
		struct Compute < T1, T2, typename enable_if <
				and_ <
				have_same_category<T1, T2>,
				or_<is_matrix<T1>, is_quat<T1> >,
				have_same_dimension<T1, T2>,
				have_compatible_scalar<T1, T2> > >::type > {
			typedef TransformCompose type;
		};

		template<typename T1, typename T2>
		struct Compute < T1, T2, typename enable_if <
				and_ <
				and_ <
				is_vector<T1>,
				is_vector<T2>,
				have_same_dimension<T1, T2>
				> ,
				has_floating_point_scalar<T1>,
				has_floating_point_scalar<T2>,
				have_compatible_scalar<T1, T2> > >::type > {
			typedef VectorDotProduct type;
		};

		template<typename T1, typename T2>
		struct Compute < T1, T2, typename enable_if <
				and_ <
				is_transformable_vector<T1>,
				is_matrix<T2>,
				has_dimension<T2, 4>,
				have_compatible_scalar<T1, T2> > >::type > {
			typedef VectorTimesMatrix type;
		};

		template<typename T1, typename T2>
		struct Compute < T1, T2, typename enable_if <
				and_ <
				is_transformable_vector<T2>,
				is_matrix<T1>,
				has_dimension<T1, 4>,
				have_compatible_scalar<T1, T2> > >::type > {
			typedef TransformTimesVector type;
		};

		template<typename T1, typename T2>
		struct Compute < T1, T2, typename enable_if <
				and_ <
				is_quat<T1>,
				is_vector<T2>,
				has_dimension<T2, 3>,
				have_compatible_scalar<T1, T2> > >::type > {
			typedef TransformTimesVector type;
		};

		template<typename T1, typename T2>
		struct Compute < T1, T2, typename enable_if <
				and_ <
				is_vector<T1>,
				is_scalar<T2>,
				have_compatible_scalar<T1, T2> > >::type > {
			typedef VectorScalar type;
		};
		template<typename T1, typename T2>
		struct Compute < T1, T2, typename enable_if <
				and_ <
				is_scalar<T1>,
				is_vector<T2>,
				have_compatible_scalar<T1, T2> > >::type > {
			typedef ScalarVector type;
		};
	}

	namespace detail {
		template<typename Tag>
		struct Multiplication_impl;

		template<typename T1, typename T2>
		struct MultiplicationSpecialization :
				Multiplication_impl<typename MultiplicationTags::Compute<T1, T2>::type>::template apply<T1, T2>,
		                    BinarySpecializedOperator<Multiplication, T1, T2> {
		                        typedef MultiplicationSpecialization<T1, T2> type;
		                    };
		template<typename Tag>
		struct Multiplication_impl {
			template<typename T1, typename T2>
			struct apply {
			};
		};

		/// Two vectors: dot product
		template<>
		struct Multiplication_impl <MultiplicationTags::VectorDotProduct> {

			template<typename T1, typename T2>
			struct apply {
				typedef typename get_compatible_scalar<T1, T2>::type return_type;

				template<typename A, typename B>
				static return_type performOperation(A const& v1, B const& v2) {
					return v1 * v2;
				}
			};
		};

		/// Same category and dimension: promote and multiply
		template<>
		struct Multiplication_impl <MultiplicationTags::TransformCompose> {

			template<typename T1, typename T2>
			struct apply {
				typedef typename promote_type_with_scalar<T1, typename get_scalar<T2>::type>::type return_type;

				static return_type performOperation(T1 const& v1, T2 const& v2) {
					return return_type(v1) * return_type(v2);
				}
			};
		};

		/// Transform vector by matrix.
		template<>
		struct Multiplication_impl <MultiplicationTags::VectorTimesMatrix> {
			template<typename V, typename M>
			struct apply {
				typedef V return_type;

				static return_type performOperation(V const& v, M const& m) {
					return v * m;
				}
			};
		};

		/// Transform vector by matrix.
		template<>
		struct Multiplication_impl <MultiplicationTags::TransformTimesVector> {
			template<typename M, typename V>
			struct apply {
				typedef V return_type;

				static return_type performOperation(M const& m, V const& v) {
					return m * v;
				}
			};
		};

		/// Scale vector
		template<>
		struct Multiplication_impl <MultiplicationTags::VectorScalar> {

			template<typename V, typename S>
			struct apply {
				typedef typename promote_type_with_scalar<V, S>::type vec_type;
				typedef vec_type return_type;

				static return_type performOperation(V const& v, S const& s) {
					return vec_type(v) * s;
				}
			};
		};

		/// Scale vector
		template<>
		struct Multiplication_impl <MultiplicationTags::ScalarVector> {

			template<typename S, typename V>
			struct apply {
				typedef typename promote_type_with_scalar<V, S>::type vec_type;
				typedef vec_type return_type;

				static return_type performOperation(S const& s, V const& v) {
					return vec_type(v) * s;
				}
			};
		};
	} // end of namespace detail

	struct Multiplication : BinaryOperatorBase {
		template<typename T1, typename T2>
		struct apply : detail::MultiplicationSpecialization<T1, T2> {
			typedef detail::MultiplicationSpecialization<T1, T2> type;
		};
	};

} // end of namespace osgTraits

#endif // INCLUDED_Multiplication_h_GUID_d7c2cc4f_5b7d_4891_bc1d_6b3a21a9b3c9
