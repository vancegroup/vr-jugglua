/*
	osgLua: use Lua to access dynamically to osg using osgIntrospection
	Copyright(C) 2006 Jose L. Hidalgo Valiño (PpluX) (pplux at pplux.com)

    This library is open source and may be redistributed and/or modified under
    the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or
    (at your option) any later version.  The full license is in LICENSE file
    included with this distribution, and on the openscenegraph.org website.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    OpenSceneGraph Public License for more details.
*/

#include <osgLua/Config>
#include <osgLua/introspection/Reflection>
#include <osgLua/introspection/MethodInfo>
#include <osgLua/introspection/ConstructorInfo>
#include <osgLua/introspection/PropertyInfo>
#include <osgLua/introspection/Utility>
#include <osgLua/introspection/Value>
#include <osgLua/introspection/Type>

#include <osgLua/LuaInclude>

namespace osgLua {

	void pushParameter(lua_State *L, const introspection::ParameterInfo *p);
	void pushMethodInfo(lua_State *L, const introspection::MethodInfo *mi);
	void pushPropertyInfo(lua_State *L, const introspection::PropertyInfo *mi);
	void pushConstructorInfo(lua_State *L,
	                         const introspection::ConstructorInfo *ci);
	void pushTypeInfo(lua_State *L, const introspection::Type *type);


} // end of osgLua namespace
