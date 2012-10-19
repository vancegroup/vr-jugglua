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
#ifndef INCLUDED_MathTypeDetail_h_GUID_f14504f4_cbc4_4d16_b09a_f8b6b0f6f8b2
#define INCLUDED_MathTypeDetail_h_GUID_f14504f4_cbc4_4d16_b09a_f8b6b0f6f8b2

// Internal Includes
// - none

// Library/third-party includes
// - none

// Standard includes
// - none

namespace osgTraits {

	template<typename CategoryTag, typename ScalarTag, typename DimensionTag>
	struct MathTypeDetail {
		typedef MathTypeDetail<CategoryTag, ScalarTag, DimensionTag> type;
		typedef CategoryTag category_tag;
		typedef ScalarTag scalar_tag;
		typedef DimensionTag dimension_tag;
	};
	/*
		template<typename T>
		struct GetMathTypeDetail {
			typedef MathTypeDetail<typename GetCategory<T>::type, typename GetScalar<T>::type, typename GetDimension<T>::type> type;
		};
	*/

} // end of namespace osgTraits
#endif // INCLUDED_MathTypeDetail_h_GUID_f14504f4_cbc4_4d16_b09a_f8b6b0f6f8b2
