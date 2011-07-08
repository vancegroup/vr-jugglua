/** @file
	@brief Header defining template metafunction class to push a type's
	qualified name as determined by osgIntrospection

	@date 2011

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

#pragma once
#ifndef INCLUDED_osgLuaBind_pushTypeName_h_GUID_90af8585_72fe_4b92_9c3c_36e2e0329520
#define INCLUDED_osgLuaBind_pushTypeName_h_GUID_90af8585_72fe_4b92_9c3c_36e2e0329520

// Internal Includes
#include "LuaInclude.h"

// Library/third-party includes
#include <osg/ref_ptr>
#include <osgLua/IntrospectionType>

// Standard includes
// - none

namespace osgLuaBind {

	template <typename T>
	struct PushTypeName {
		static void apply(lua_State * L) {
			static const osgIntrospection::Type& destType =
				osgIntrospection::Reflection::getType(extended_typeid<T>());

			lua_pushstring(L, destType.getQualifiedName().c_str());
		}
	}

	template <typename T>
	struct PushTypeName< ::osg::ref_ptr<T> > {
		static void apply(lua_State * L) {
			lua_pushstring(L, "osg::ref_ptr<");
			PushTypeName<T>::apply(L);
			lua_pushstring(L, ">");
			lua_concat(L, 3);
		}
	}

} // end of namespace osgLuaBind

#endif // INCLUDED_osgLuaBind_pushTypeName_h_GUID_90af8585_72fe_4b92_9c3c_36e2e0329520
