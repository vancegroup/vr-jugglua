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
#ifndef INCLUDED_ArithmeticTypes_h_GUID_ddc568fc_99ac_4121_a909_f1831bbd9371
#define INCLUDED_ArithmeticTypes_h_GUID_ddc568fc_99ac_4121_a909_f1831bbd9371

// Internal Includes
// - none

// Library/third-party includes
#include <boost/mpl/list/list10.hpp>

// Standard includes
// - none

namespace osgTraits {
	/// @todo this list is incomplete
	typedef boost::mpl::list7<double, float, long int, int, short, signed char, unsigned char> arithmetic_types;
} // end of namespace osgTraits

#endif // INCLUDED_ArithmeticTypes_h_GUID_ddc568fc_99ac_4121_a909_f1831bbd9371
