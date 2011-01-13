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

#include "Value.h"
#include "Type.h"

#include <osgIntrospection/Reflection>
#include <osgIntrospection/MethodInfo>
#include <osgIntrospection/ConstructorInfo>
#include <osgIntrospection/Utility>
#include <osgIntrospection/Value>
#include <osgIntrospection/Type>

#include <iostream>
#include <sstream>
#include <vrjugglua/LuaIncludeFull.h>

namespace osgLua {

	namespace Type
	{
		void push(lua_State *L, const char *TypeName);
		int index(lua_State *L);
		int call(lua_State *L);
	}

	int staticMethodCall(lua_State *L)
	{
		int top = lua_gettop(L);
		const char *tname  = lua_tostring(L, lua_upvalueindex(1));
		const char *method = lua_tostring(L, lua_upvalueindex(2));
		try
		{
			const osgIntrospection::Type &type = 
				osgIntrospection::Reflection::getType(tname);

			osgIntrospection::ValueList vl;
			for(int i = 1; i <= top; ++i)
			{
				vl.push_back( getValue(L, i) );
			}

			osgIntrospection::ParameterInfoList params;
			const osgIntrospection::MethodInfo *mi =
				type.getCompatibleMethod(method,vl,false);

			if (mi)
			{
				Value::push(L, mi->invoke(vl));
				return 1;
			}
/** @todo remove this old enum code? 
			// Maybe it's an enum
			if (type.isEnum())
			{
				// create a table with the values
				const osgIntrospection::EnumLabelMap &map =
					type.getEnumLabels();
				for( osgIntrospection::EnumLabelMap::const_iterator
					i = map.begin(); i != map.end(); ++i)
				{
					if (i->second == method) {

						osgIntrospection::Value v;
						std::istringstream s(i->second);
						std::stringstream os;

						type.getReaderWriter()->readTextValue(s, v);

						Value::push(L, v);
						return 1;
					}
				}
			}
*/

			luaL_error(L, "Class %s do not have static method %s",
				tname,method);
		}
		catch (osgIntrospection::Exception &e)
		{
			luaL_error(L,"[%s:%d] %s",__FILE__,__LINE__,e.what().c_str());
		}
		return 0;
	}

	void Type::push(lua_State *L, const char *TypeName)
	{
		lua_newtable(L); // table
		lua_newtable(L); // metatable
		lua_pushstring(L, TypeName);
		lua_pushcclosure(L, Type::index, 1);
		lua_setfield(L, -2, "__index");
		lua_pushstring(L, TypeName);
		lua_pushcclosure(L, Type::call, 1);
		lua_setfield(L, -2, "__call");
		lua_setmetatable(L, -2);
	}

	int Type::index(lua_State *L)
	{
		std::string tbase( lua_tostring(L, lua_upvalueindex(1)) );
		const char *name = luaL_checkstring(L, 2);
		std::string base(tbase+"::"+name);

		// traverse the namespace
		try
		{
			
			const osgIntrospection::Type &type =
				osgIntrospection::Reflection::getType(base);

			// is it an enum ?
			/// @todo Make methods that require an enum accept a long

			if (type.isEnum())
			{
				// create a table with the values
				const osgIntrospection::EnumLabelMap &map =
					type.getEnumLabels();
				//Type::push(L, base.c_str());

				lua_newtable(L);
				for( osgIntrospection::EnumLabelMap::const_iterator
					i = map.begin(); i != map.end(); ++i)
				{
					lua_pushinteger(L, i->first);

					lua_setfield(L, -2, i->second.c_str());
				}
				return 1;
			}


			// if not... push the type
			Type::push(L, base.c_str());
			return 1;
		}
		catch (osgIntrospection::TypeNotFoundException&)
		{
			// if base is not a valid type... supose that
			// tbase.name is an static function 
			lua_pushstring(L,tbase.c_str());
			lua_pushstring(L,name);
			lua_pushcclosure(L, staticMethodCall, 2);
			return 1;
		}

	}

	int Type::call(lua_State *L)
	{
		int top = lua_gettop(L);
		std::string name( lua_tostring(L, lua_upvalueindex(1)) );
		try
		{
			const osgIntrospection::Type &type = 
				osgIntrospection::Reflection::getType(name);
		
			osgIntrospection::ValueList vl;
			for(int i = 2; i <= top; ++i)
			{
				vl.push_back( getValue(L, i) );
			}

			osgIntrospection::Value returnedval = type.createInstance(vl);

			Value::push(L, returnedval);
			return 1;
		}
		catch (osgIntrospection::Exception &e)
		{
			luaL_error(L,"[%s:%d] %s",__FILE__,__LINE__,e.what().c_str());
		}
		return 0;
	}

} // end of osgLua namespace
