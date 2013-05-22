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
#include <osgLua/Value>

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
	struct osgLuaValueUsableImpl {
		static bool check(lua_State * L, int i) {
			bool ret = false;
			Value * a = Value::get(L, i);
			if (a) {
				ret = typeUsableAs<T>(a->get().getType());
			}
			return ret;
		}
	};
	template<>
	bool osgLuaValueUsableImpl<double>::check(lua_State * L, int i) {
		return lua_isnumber(L, i);
	}

	template<>
	bool osgLuaValueUsableImpl<float>::check(lua_State * L, int i) {
		return lua_isnumber(L, i);
	}

	template<typename T>
	inline bool osgLuaValueUsableAs(lua_State * L, int i) {
		return osgLuaValueUsableImpl<T>::check(L, i);
	}



} // end of namespace osgLua

#endif // INCLUDED_UsableAs_h_GUID_f17dbcc1_8413_44f5_9b69_f91bc137bbaa
