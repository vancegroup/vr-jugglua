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
#include "OsgMathTraits.h"
#include "SelectType.h"
#include "PromoteTypeWithScalar.h"
#include "OperatorBase.h"
#include "TypePredicates.h"

// Library/third-party includes
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/and.hpp>

// Standard includes
// - none


namespace osgTraits {
	namespace MultiplicationTags {
		using namespace ::osgTraits::BinaryPredicates;
		using boost::enable_if;
		using boost::mpl::and_;

		struct SameCategoryAndDimensionWithCompatibleScalar {};
		struct VectorTimesMatrix {};
		struct MatrixTimesVector {};

		struct VectorScalar {};
		struct ScalarVector {};

		template<typename T1, typename T2, typename = void>
		struct Compute {
			typedef void type;
		};

		template<typename T1, typename T2>
		struct Compute<T1, T2, typename enable_if<typename HaveSameCategoryAndDimensionWithCompatibleScalar<T1, T2>::type >::type > {
			typedef SameCategoryAndDimensionWithCompatibleScalar type;
		};

		template<typename T1, typename T2>
		struct Compute < T1, T2, typename enable_if <
				typename and_ <
				AreVectorAndMatrix<T1, T2>,
				CanTransformVecMatrix<T1, T2>,
				HaveCompatibleScalar<T1, T2> >::type >::type > {
			typedef VectorTimesMatrix type;
		};

		template<typename T1, typename T2>
		struct Compute < T1, T2, typename enable_if <
				typename and_ <
				AreVectorAndMatrix<T2, T1>,
				CanTransformVecMatrix<T2, T1>,
				HaveCompatibleScalar<T2, T1> >::type >::type > {
			typedef MatrixTimesVector type;
		};

		template<typename T1, typename T2>
		struct Compute < T1, T2, typename enable_if <
				typename and_ <
				is_vector<T1>,
				is_scalar<T2>,
				HaveCompatibleScalar<T1, T2> >::type >::type > {
			typedef VectorScalar type;
		};
		template<typename T1, typename T2>
		struct Compute < T1, T2, typename enable_if <
				typename and_ <
				is_scalar<T1>,
				is_vector<T2>,
				HaveCompatibleScalar<T1, T2> >::type >::type > {
			typedef ScalarVector type;
		};
	}

	namespace detail {
		template<typename Tag>
		struct Multiplication_impl {
			template<typename T1, typename T2>
			struct apply {
				typedef boost::mpl::false_ available;
				typedef void unavailable;
			};
		};

		template<typename T1, typename T2>
		struct Multiplication :
				Multiplication_impl<typename MultiplicationTags::Compute<T1, T2>::type>::template apply<T1, T2>,
		                    BinaryOperatorBase<T1, T2> {};

		/// Same category and dimension: promote and multiply
		template<>
		struct Multiplication_impl <MultiplicationTags::SameCategoryAndDimensionWithCompatibleScalar> {

			template<typename T1, typename T2>
			struct apply {
				typedef boost::mpl::true_ available;
				typedef typename PromoteTypeWithScalar<T1, typename GetScalar<T2>::type>::type result_type;

				template<typename A, typename B>
				static result_type performOperation(A const& v1, B const& v2) {
					return result_type(v1) * result_type(v2);
				}
			};
		};

		/// Transform vector by matrix.
		template<>
		struct Multiplication_impl <MultiplicationTags::VectorTimesMatrix> {
			template<typename V, typename M>
			struct apply {
				typedef boost::mpl::true_ available;
				typedef V result_type;

				static result_type performOperation(V const& v, M const& m) {
					return v * m;
				}
			};
		};

		/// Transform vector by matrix.
		template<>
		struct Multiplication_impl <MultiplicationTags::MatrixTimesVector> {
			template<typename M, typename V>
			struct apply {
				typedef boost::mpl::true_ available;
				typedef V result_type;

				static result_type performOperation(M const& m, V const& v) {
					return m * v;
				}
			};
		};

		/// Scale vector
		template<>
		struct Multiplication_impl <MultiplicationTags::VectorScalar> {

			template<typename V, typename S>
			struct apply {
				typedef boost::mpl::true_ available;
				typedef typename PromoteTypeWithScalar<V, S>::type vec_type;
				typedef vec_type result_type;

				static result_type performOperation(S const& s, V const& v) {
					return vec_type(v) * s;
				}
			};
		};

		/// Scale vector
		template<>
		struct Multiplication_impl <MultiplicationTags::ScalarVector> {

			template<typename S, typename V>
			struct apply {
				typedef boost::mpl::true_ available;
				typedef typename PromoteTypeWithScalar<V, S>::type vec_type;
				typedef vec_type result_type;

				static result_type performOperation(V const& v, S const& s) {
					return vec_type(v) * s;
				}
			};
		};
	} // end of namespace detail

	struct Multiplication : BinaryOperatorClassBase {
		template<typename T1, typename T2>
		struct apply {
			typedef detail::Multiplication<T1, T2> type;
		};
	};

} // end of namespace osgTraits

#endif // INCLUDED_Multiplication_h_GUID_d7c2cc4f_5b7d_4891_bc1d_6b3a21a9b3c9
