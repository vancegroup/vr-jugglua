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
#ifndef INCLUDED_UsableAs_h_GUID_f17dbcc1_8413_44f5_9b69_f91bc137bbaa
#define INCLUDED_UsableAs_h_GUID_f17dbcc1_8413_44f5_9b69_f91bc137bbaa

// Internal Includes
// - none

// Library/third-party includes
#include <osgLua/introspection/ExtendedTypeInfo>
#include <osgLua/introspection/Value>
#include <osgLua/introspection/Type>
#include <osgLua/introspection/variant_cast>

// Standard includes
// - none

namespace osgLua {

	template<typename T>
	inline bool typeUsableAs(const introspection::Type& t) {
		const introspection::Type& myType =
		    introspection::Reflection::getType(extended_typeid<T>());
		return t == myType || t.isSubclassOf(myType);
	}

	template<typename T>
	inline bool osgLuaValueUsableAs(lua_State * L, int i) {
		Value * a = Value::get(L, i);
		if (a) {
			return typeUsableAs<T>(a->get().getType());
		}
		return false;
	}

} // end of namespace osgLua

#endif // INCLUDED_UsableAs_h_GUID_f17dbcc1_8413_44f5_9b69_f91bc137bbaa
