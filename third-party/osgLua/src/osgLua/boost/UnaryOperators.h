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
#ifndef INCLUDED_UnaryOperators_h_GUID_e148c336_6ecb_4cad_9bfa_5a975e12fe59
#define INCLUDED_UnaryOperators_h_GUID_e148c336_6ecb_4cad_9bfa_5a975e12fe59


// Internal Includes
//#include "UnaryMinus.h"

// Library/third-party includes
//#include <boost/mpl/single_view.hpp>
#include <boost/mpl/vector/vector10.hpp>

// Standard includes
// - none

namespace osgTraits {
	//typedef boost::mpl::single_view<UnaryMinus> UnaryOperators;
	typedef boost::mpl::vector0<> UnaryOperators;
} // end of namespace osgTraits

#endif // INCLUDED_UnaryOperators_h_GUID_e148c336_6ecb_4cad_9bfa_5a975e12fe59
