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
#include "CompatibleScalar.h"
#include "SelectType.h"
#include "PromoteTypeWithScalar.h"

// Library/third-party includes
#include <boost/mpl/bool.hpp>

// Standard includes
// - none


namespace osgTraits {
	namespace detail {
		template<typename T1, typename T2, typename = void>
		struct MultiplicationImpl {
			typedef void unavailable;
		};


		/// Same category and dimension: promote and multiply
		template<typename T1, typename T2>
		struct MultiplicationImpl < T1, T2,
				typename boost::enable_if <
				boost::mpl::and_ <
				boost::is_same<typename GetCategory<T1>::type, typename GetCategory<T2>::type>,
				TypesHaveCompatibleScalar<T1, T2>,
				boost::is_same<typename GetDimension<T1>::type, typename GetDimension<T2>::type>
				>
				>::type > {
			typedef typename PromoteTypeWithScalar<T1, typename GetValueType<T2>::type>::type result_type;

			template<typename A, typename B>
			static result_type apply(T1 const& v1, T2 const& v2) {
				return result_type(v1) * result_type(v2);
			}

		};

		/// Transform vector by matrix.
		template<typename V, typename M>
		struct MultiplicationImpl < V, M,
				typename boost::enable_if <
				boost::mpl::and_ <
				boost::is_same<typename GetCategory<V>::type, tags::Vec>,
				boost::is_same<typename GetCategory<M>::type, tags::Matrix>,
				TypesHaveCompatibleScalar<V, M>
				>
				>::type > {

			typedef V result_type;

			template<typename T1, typename T2>
			static result_type apply(T1 const& v, T2 const& m) {
				return v * m;
			}
		};

		/// Transform vector by matrix.
		template<typename M, typename V>
		struct MultiplicationImpl < M, V,
				typename boost::enable_if <
				boost::mpl::and_ <
				boost::is_same<typename GetCategory<M>::type, tags::Matrix>,
				boost::is_same<typename GetCategory<V>::type, tags::Vec>,
				TypesHaveCompatibleScalar<M, V>
				>
				>::type > {

			typedef V result_type;

			template<typename T1, typename T2>
			static result_type apply(T1 const& m, T2 const& v) {
				return m * v;
			}
		};

		/// Scale vector
		template<typename S, typename V>
		struct MultiplicationImpl < S, V,
				typename boost::enable_if <
				boost::mpl::and_ <
				boost::is_arithmetic<S, tags::Matrix>,
				CompatibleScalars<S, typename GetScalar<V>::type>,
				boost::is_same<typename GetCategory<V>::type, tags::Vec>
				>
				>::type > {

			typedef typename PromoteTypeWithScalar<V, S>::type vec_type;
			typedef vec_type result_type;

			template<typename T1, typename T2>
			static result_type apply(T1 const& s, T2 const& v) {
				return vec_type(v) * s;
			}
		};

		/// Scale vector
		template<typename V, typename S>
		struct MultiplicationImpl < V, S,
				typename boost::enable_if <
				boost::mpl::and_ <
				boost::is_arithmetic<S, tags::Matrix>,
				CompatibleScalars<S, typename GetScalar<V>::type>,
				boost::is_same<typename GetCategory<V>::type, tags::Vec>
				>
				>::type > {

			typedef typename PromoteTypeWithScalar<V, S>::type vec_type;
			typedef vec_type result_type;

			template<typename T1, typename T2>
			static result_type apply(T1 const& v, T2 const& s) {
				return vec_type(v) * s;
			}
		};
	} // end of namespace detail


	template<typename T1, typename T2>
	struct Multiplication : detail::MultiplicationImpl<T1, T2> {};

} // end of namespace osgTraits

#endif // INCLUDED_Multiplication_h_GUID_d7c2cc4f_5b7d_4891_bc1d_6b3a21a9b3c9
