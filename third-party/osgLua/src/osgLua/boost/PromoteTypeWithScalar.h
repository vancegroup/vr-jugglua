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
#ifndef INCLUDED_PromoteTypeWithScalar_h_GUID_0b6b2500_1f6a_4a70_b11a_74eb92b91885
#define INCLUDED_PromoteTypeWithScalar_h_GUID_0b6b2500_1f6a_4a70_b11a_74eb92b91885

// Internal Includes
#include "OsgMathTraits.h"
#include "SelectType.h"
#include "GetCompatibleScalar.h"

// Library/third-party includes
// - none

// Standard includes
// - none


namespace osgTraits {

	template<typename T, typename ScalarType>
	struct PromoteTypeWithScalar {
		typedef typename GetCategory<T>::type Category;
		typedef typename GetCompatibleScalar<typename GetScalar<T>::type, ScalarType>::scalar_type Scalar;
		typedef typename GetDimension<T>::type Dimension;
		typedef typename SelectType< Category, Scalar, Dimension>::type type;
	};

} // end of namespace osgTraits
#endif // INCLUDED_PromoteTypeWithScalar_h_GUID_0b6b2500_1f6a_4a70_b11a_74eb92b91885
