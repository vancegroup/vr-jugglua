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
// - none

// Standard includes
// - none

namespace osgTraits {
	/*
	BOOST_MPL_HAS_XXX_TRAIT_DEF(unavailable);

	template<typename SpecOperator>
	struct is_operator_available : boost::mpl::not_<typename has_unavailable<SpecOperator>::type>  {};

	template<typename SpecOperator, typename = void>
	struct is_operator_available<SpecOperator, bo>

	template<typename SpecOperator>
	struct is_operator_available<SpecOperator, typename SpecOperator::unavailable> : boost::mpl::false_ {};
	*/

	template<typename SpecOperator>
	struct is_operator_available : SpecOperator::available {};
} // end of namespace osgTraits

#endif // INCLUDED_IsOperatorAvailable_h_GUID_ea6e8ae3_7a30_4c88_b99e_5f12b40ee59b
