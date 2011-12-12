/*
	osgLua: use Lua to access dynamically to osg using osgIntrospection
	Copyright(C) 2006 Jose L. Hidalgo Valiño (PpluX) (pplux at pplux.com)
	Copyright(C) 2010-2011 Iowa State University (Author: Ryan Pavlik <rpavlik@acm.org> )

    This library is open source and may be redistributed and/or modified under
    the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or
    (at your option) any later version.  The full license is in LICENSE file
    included with this distribution, and on the openscenegraph.org website.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    OpenSceneGraph Public License for more details.
*/

#include "Type.h"

#include <osgLua/Value>                 // for getValue, Value

#include "LuaIncludeFull.h"

#include <osgLua/introspection/Reflection>
#include <osgLua/introspection/MethodInfo>
#include <osgLua/introspection/Exceptions>
#include <osgLua/introspection/Value>    // for ValueList, Value
#include <osgLua/introspection/Type>     // for EnumLabelMap, Type, etc

#include <map>                          // for _Rb_tree_const_iterator, etc
#include <string>                       // for string, basic_string, etc
#include <utility>                      // for pair, make_pair


namespace osgLua {

	namespace Type {
		void push(lua_State *L, const char *TypeName);
		int index(lua_State *L);
		int call(lua_State *L);
	}

	const introspection::Type & lookupType(std::string const& name) {
		typedef std::map<std::string, const introspection::Type *> TypeNameMap;
		static TypeNameMap usedTypes;
		TypeNameMap::iterator it = usedTypes.find(name);
		if (it != usedTypes.end()) {
			return *(it->second);
		}
		const introspection::Type &type =
		    introspection::Reflection::getType(name);
		usedTypes.insert(std::make_pair(name, &type));
		return type;
	}

	int staticMethodCall(lua_State *L) {
		int top = lua_gettop(L);
		const char *tname  = lua_tostring(L, lua_upvalueindex(1));
		const char *method = lua_tostring(L, lua_upvalueindex(2));
		try {
			const introspection::Type &type =
			    introspection::Reflection::getType(tname);

			introspection::ValueList vl;
			for (int i = 1; i <= top; ++i) {
				vl.push_back(getValue(L, i));
			}

			introspection::ParameterInfoList params;
			const introspection::MethodInfo *mi =
			    type.getCompatibleMethod(method, vl, false);

			if (mi) {
				Value::push(L, mi->invoke(vl));
				return 1;
			}
			/** @todo remove this old enum code?
						// Maybe it's an enum
						if (type.isEnum())
						{
							// create a table with the values
							const introspection::EnumLabelMap &map =
								type.getEnumLabels();
							for( introspection::EnumLabelMap::const_iterator
								i = map.begin(); i != map.end(); ++i)
							{
								if (i->second == method) {

									introspection::Value v;
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
			           tname, method);
		} catch (introspection::Exception &e) {
			luaL_error(L, "[%s:%d] %s", __FILE__, __LINE__, e.what().c_str());
		}
		return 0;
	}

	void Type::push(lua_State *L, const char *TypeName) {
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

	int Type::index(lua_State *L) {
		std::string tbase(lua_tostring(L, lua_upvalueindex(1)));
		const char *name = luaL_checkstring(L, 2);
		std::string base(tbase + "::" + name);

		// traverse the namespace
		try {

			const introspection::Type &type = lookupType(base);
			//introspection::Reflection::getType(base);

			// is it an enum ?
			/// @todo Make methods that require an enum accept a long

			if (type.isEnum()) {
				// create a table with the values
				const introspection::EnumLabelMap &map =
				    type.getEnumLabels();
				//Type::push(L, base.c_str());

				lua_newtable(L);
				for (introspection::EnumLabelMap::const_iterator
				        i = map.begin(); i != map.end(); ++i) {
					lua_pushinteger(L, i->first);

					lua_setfield(L, -2, i->second.c_str());
				}
				return 1;
			}


			// if not... push the type
			Type::push(L, base.c_str());
			return 1;
		} catch (introspection::TypeNotFoundException&) {
			// if base is not a valid type... suppose that
			// tbase.name is an static function
			lua_pushstring(L, tbase.c_str());
			lua_pushstring(L, name);
			lua_pushcclosure(L, staticMethodCall, 2);
			return 1;
		} catch (introspection::TypeNotDefinedException &) {
			return luaL_error(L, "Type '%s' does not have a wrapper defined, so it is not accessible through osgLua.", base.c_str());
		}
	}

	int Type::call(lua_State *L) {
		int top = lua_gettop(L);
		std::string name(lua_tostring(L, lua_upvalueindex(1)));
		try {
			const introspection::Type &type = lookupType(name);
			//introspection::Reflection::getType(name);

			introspection::ValueList vl;
			for (int i = 2; i <= top; ++i) {
				vl.push_back(getValue(L, i));
			}

			introspection::Value returnedval = type.createInstance(vl);

			Value::push(L, returnedval);
			return 1;
		} catch (introspection::Exception &e) {
			luaL_error(L, "[%s:%d] %s", __FILE__, __LINE__, e.what().c_str());
		}
		return 0;
	}

} // end of osgLua namespace
