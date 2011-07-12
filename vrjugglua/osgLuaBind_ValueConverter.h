/** @file
	@brief Header

	@date 2011

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

#pragma once
#ifndef INCLUDED_osgLuaBind_ValueConverter_h_GUID_623e745d_7cf3_4c59_90b6_300045a2bfbf
#define INCLUDED_osgLuaBind_ValueConverter_h_GUID_623e745d_7cf3_4c59_90b6_300045a2bfbf

// Internal Includes
#include "osgLuaBind_ConverterBase.h"
#include "osgLuaBind_isValueType.h"

// Library/third-party includes
// - none

// Standard includes
// - none

namespace luabind {
	template <typename T, typename Enable>
	struct default_converter;

	/// OSG values should be converted as such
	template <typename T>
	struct default_converter<T, typename osgLuaBind::IsValueType<T>::type>
			: osgLuaBind::detail::ConverterBase<T> {};
} // end of namespace luabind

#endif // INCLUDED_osgLuaBind_ValueConverter_h_GUID_623e745d_7cf3_4c59_90b6_300045a2bfbf
