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
#ifndef INCLUDED_GetDimension_h_GUID_af8b122d_2445_43da_82c3_22da8a160310
#define INCLUDED_GetDimension_h_GUID_af8b122d_2445_43da_82c3_22da8a160310

// Internal Includes
#include "ComputeCategoryTag.h"
#include "Tags.h"

// Library/third-party includes
#include <boost/mpl/int.hpp>

// Standard includes
// - none


namespace osgTraits {
	namespace detail {
		template<typename CategoryTag>
		struct GetDimension_impl {
			template<typename T>
			struct apply {
				typedef void type;
			};
		};
		template<>
		struct GetDimension_impl<tags::Vec> {
			template<typename T>
			struct apply {
				typedef boost::mpl::int_<T::num_components> type;
			};
		};

		template<>
		struct GetDimension_impl<tags::Matrix> {
			template<typename T>
			struct apply {
				typedef boost::mpl::int_<4> type;
			};
		};

		template<>
		struct GetDimension_impl<tags::Quat> {
			template<typename T>
			struct apply {
				typedef boost::mpl::int_<4> type;
			};
		};

		template<>
		struct GetDimension_impl<tags::Scalar> {
			template<typename T>
			struct apply {
				typedef boost::mpl::int_<1> type;
			};
		};

	} // end of namespace detail

	template<typename T>
	struct GetDimension : detail::GetDimension_impl<typename detail::ComputeCategoryTag<T>::type >::template apply<T> {};

} // end of namespace osgTraits

#endif // INCLUDED_GetDimension_h_GUID_af8b122d_2445_43da_82c3_22da8a160310
