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

#include "lua_functions.h"


namespace osgLua {

	const char *getName(const osgIntrospection::Type &type)
	{
		if (type.isDefined())
		{
			return type.getName().c_str();
		}
		return "(not defined)";
	}

	void pushParameter(lua_State *L, const osgIntrospection::ParameterInfo *p)
	{
		const char *in = "IN";
		const char *out = "OUT";
		const char *inout = "IN/OUT";

		const char *type = in;
		if (p->isInOut()) type = inout;
		else if (p->isOut()) type = out;

		lua_pushfstring(L, "[%s] %s %s",type, 
			getName(p->getParameterType()),
			p->getName().c_str()
			);
	}

	void pushMethodInfo(lua_State *L, const osgIntrospection::MethodInfo *mi)
	{
		int top = lua_gettop(L);
		lua_pushfstring(L, "%s %s(",
			getName(mi->getReturnType()),
			mi->getName().c_str());
		const osgIntrospection::ParameterInfoList &param = mi->getParameters();
		for (osgIntrospection::ParameterInfoList::const_iterator 
				i = param.begin(); i != param.end(); ++i)
		{
			pushParameter(L, *i);
			lua_pushstring(L,",");
		}
		if (!param.empty()) lua_pop(L,1);
		lua_pushstring(L, ")");
		if (mi->isConst()) lua_pushstring(L, " const");
		if (mi->isStatic()) lua_pushstring(L, " static");
		lua_concat(L, lua_gettop(L) - top);
	}
	
	void pushConstructorInfo(lua_State *L, 
		const osgIntrospection::ConstructorInfo *ci)
	{
		int top = lua_gettop(L);
		lua_pushfstring(L, "%s (",
			getName(ci->getDeclaringType()));
		const osgIntrospection::ParameterInfoList &param = ci->getParameters();
		for (osgIntrospection::ParameterInfoList::const_iterator 
				i = param.begin(); i != param.end(); ++i)
		{
			pushParameter(L, *i);
			lua_pushstring(L,",");
		}
		if (!param.empty()) lua_pop(L,1);
		lua_pushstring(L, ")");
		lua_concat(L, lua_gettop(L) - top);
	}
	
	void pushTypeInfo(lua_State *L, const osgIntrospection::Type *type)
	{
		if (type)
		{
			lua_newtable(L);
			int table = lua_gettop(L);

			{ // Stdname
				lua_pushstring(L,  type->getStdTypeInfo().name());
				lua_setfield(L,table, "stdName");
			}
			{ // Type_pointer
				lua_pushinteger(L, (lua_Integer)(void*) type);
				lua_setfield(L,table, "typePointer");
			}

			if (!type->isDefined()) return;

			{ // name
				lua_pushstring(L,  type->getQualifiedName().c_str() );
				lua_setfield(L,table, "name");
			}
			{ // methods
				lua_newtable(L);
				int count = 1;
				const osgIntrospection::MethodInfoList &list = 
					type->getMethods();
				for(osgIntrospection::MethodInfoList::const_iterator
					i = list.begin(); i != list.end(); ++i, ++count)
				{
					pushMethodInfo(L, *i);
					lua_rawseti(L,-2, count);
				}
				lua_setfield(L, table, "methods");
			}
			{ // constructors
				lua_newtable(L);
				int count = 1;
				const osgIntrospection::ConstructorInfoList &list = 
					type->getConstructors();
				for(osgIntrospection::ConstructorInfoList::const_iterator
					i = list.begin(); i != list.end(); ++i, ++count)
				{
					pushConstructorInfo(L, *i);
					lua_rawseti(L,-2, count);
				}
				lua_setfield(L, table, "constructors");
			}
			/// @todo add properties here
		}
		else
		{
			lua_pushnil(L);
		}
	}

} // end of osgLua namespace
