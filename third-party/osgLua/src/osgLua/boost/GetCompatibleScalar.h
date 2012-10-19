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
#ifndef INCLUDED_GetCompatibleScalar_h_GUID_d27674ff_6057_4136_9268_f854370bc94c
#define INCLUDED_GetCompatibleScalar_h_GUID_d27674ff_6057_4136_9268_f854370bc94c

// Internal Includes
#include "GetScalar.h"

// Library/third-party includes
#include <boost/mpl/bool.hpp>

// Standard includes
// - none

namespace osgTraits {
	namespace detail {
		template<typename T1, typename T2>
		struct GetCompatibleScalarImpl {};
		template<typename T>
		struct GetCompatibleScalarImpl<T, T> {
			typedef T type;
		};

		template<>
		struct GetCompatibleScalarImpl<float, double> {
			typedef double type;
		};

		template<>
		struct GetCompatibleScalarImpl<double, float> {
			typedef double type;
		};
	}
	template<typename T1, typename T2>
	struct GetCompatibleScalar : detail::GetCompatibleScalarImpl<typename GetScalar<T1>::type, typename GetScalar<T2>::type> {};



};

#endif // INCLUDED_GetCompatibleScalar_h_GUID_d27674ff_6057_4136_9268_f854370bc94c
