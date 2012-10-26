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
#include "GetCompatibleScalar.h"
#include "SelectType.h"

// Library/third-party includes
// - none

// Standard includes
// - none


namespace osgTraits {

	template<typename T, typename ScalarType>
	struct promote_type_with_scalar {
		typedef typename get_category<T>::type Category;
		typedef typename boost::mpl::eval_if < typename have_compatible_scalar<T, ScalarType>::type,
		        get_compatible_scalar<typename get_scalar<T>::type, ScalarType>,
		        get_scalar<T> >::type Scalar;
		typedef typename get_dimension<T>::type Dimension;
		typedef typename select_type< Category, Scalar, Dimension>::type type;
	};

} // end of namespace osgTraits
#endif // INCLUDED_PromoteTypeWithScalar_h_GUID_0b6b2500_1f6a_4a70_b11a_74eb92b91885
