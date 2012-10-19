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
#ifndef INCLUDED_IsOperatorAvailable_h_GUID_ea6e8ae3_7a30_4c88_b99e_5f12b40ee59b
#define INCLUDED_IsOperatorAvailable_h_GUID_ea6e8ae3_7a30_4c88_b99e_5f12b40ee59b

// Internal Includes
// - none

// Library/third-party includes
#include <boost/mpl/bool.hpp>

// Standard includes
// - none

namespace osgTraits {
	/*
	template<typename SpecOperator>
	struct is_operator_available : SpecOperator::available {};
	*/
	template<typename SpecOperator, typename = void>
	struct is_operator_available : boost::mpl::false_ {};

	template<typename SpecOperator>
	struct is_operator_available<SpecOperator, typename SpecOperator::return_type> : boost::mpl::true_ {};
} // end of namespace osgTraits

#endif // INCLUDED_IsOperatorAvailable_h_GUID_ea6e8ae3_7a30_4c88_b99e_5f12b40ee59b
