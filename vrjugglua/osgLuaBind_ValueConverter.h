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

//          Copyright Iowa State University 2010-2011.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#pragma once
#ifndef INCLUDED_osgLuaBind_ValueConverter_h_GUID_623e745d_7cf3_4c59_90b6_300045a2bfbf
#define INCLUDED_osgLuaBind_ValueConverter_h_GUID_623e745d_7cf3_4c59_90b6_300045a2bfbf

// Internal Includes
#include "osgLuaBind_ConverterBase.h"
#include "osgLuaBind_isValueType.h"
#include "osgLuaBind_pushTypeName.h"

// Library/third-party includes
#include <luabind/lua_include.hpp>

// Standard includes
// - none

namespace luabind {
	template <typename T, typename Enable>
	struct default_converter;

	/// OSG values should be converted as such
	template <typename T>
	struct default_converter<T, typename osgLuaBind::IsValueType<T>::type>
			: osgLuaBind::detail::ConverterBase<T> {};

	namespace detail {
		template <typename T, typename Enable>
		struct type_to_string;

		/// Types flagged as being OSG value types get a human-readable
		/// name string from osgIntrospection with an annotation mentioning
		/// osgLuaBind.
		template <typename T>
		struct type_to_string < T,
				typename osgLuaBind::IsValueType<T>::type
				> {
			static void get(lua_State *L) {
				lua_pushstring(L, "[osgLuaBind value] ");
				osgLuaBind::PushTypeName<T>::apply(L);
				lua_concat(L, 2);
			}
		};
	} // end of namespace detail
} // end of namespace luabind

#endif // INCLUDED_osgLuaBind_ValueConverter_h_GUID_623e745d_7cf3_4c59_90b6_300045a2bfbf
