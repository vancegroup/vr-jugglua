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
#ifndef INCLUDED_MathAndArithmeticTypes_h_GUID_2b0da401_d520_469a_abb3_e52f3c1e41aa
#define INCLUDED_MathAndArithmeticTypes_h_GUID_2b0da401_d520_469a_abb3_e52f3c1e41aa

// Internal Includes
#include "MathTypes.h"
#include "ArithmeticTypes.h"
#include "CopyToFlatSequence.h"

// Library/third-party includes
#include <boost/mpl/joint_view.hpp>
#include <boost/mpl/front_inserter.hpp>

// Standard includes
// - none

namespace osgTraits {

	typedef detail::copy_to_flat_sequence < boost::mpl::joint_view<math_types, arithmetic_types> >::type math_and_arithmetic_types;

} // end of namespace osgTraits

#endif // INCLUDED_MathAndArithmeticTypes_h_GUID_2b0da401_d520_469a_abb3_e52f3c1e41aa
