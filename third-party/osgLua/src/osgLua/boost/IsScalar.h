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
#ifndef INCLUDED_IsScalar_h_GUID_f39c9421_6a16_4a5e_805f_e5b2c9cbd2d2
#define INCLUDED_IsScalar_h_GUID_f39c9421_6a16_4a5e_805f_e5b2c9cbd2d2

// Internal Includes
// - none

// Library/third-party includes
#include <boost/type_traits/is_arithmetic.hpp>

// Standard includes
// - none

namespace osgTraits {
	/// @todo this is inconsistent with ArithmeticTypes.h
	template<typename T>
	struct is_scalar : boost::is_arithmetic<T>::type {};

} // end of namespace osgTraits
#endif // INCLUDED_IsScalar_h_GUID_f39c9421_6a16_4a5e_805f_e5b2c9cbd2d2
