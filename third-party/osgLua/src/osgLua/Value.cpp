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

#include <osgLua/Value>
#include "Value_rawget.inl"
#include "lua_functions.h"

#include "LuaIncludeFull.h"

#include <osgLua/introspection/Reflection>
#include <osgLua/introspection/Exceptions>
#include <osgLua/introspection/Utility>
#include <osgLua/introspection/ExtendedTypeInfo>
#include <osgLua/introspection/variant_cast>

#include <osg/Referenced>
#include <osg/ref_ptr>

#include <map>                          // for _Rb_tree_const_iterator, etc
#include <string>                       // for basic_string, string
#include <utility>                      // for pair

namespace osgLua {
	namespace detail {

		/// Template used to designate lua equivalents of types.
		template<typename T>
		struct LuaEquivalent;

		/// shared definition for types convertible to lua_Number
		struct is_lua_Number {
			typedef lua_Number type;
		};

		/// shared definition for types convertible to lua_Integer
		struct is_lua_Integer {
			typedef lua_Integer type;
		};

		template<>
		struct LuaEquivalent<double> : is_lua_Number {};

		template<>
		struct LuaEquivalent<float> : is_lua_Number {};

		template<>
		struct LuaEquivalent<int> : is_lua_Integer {};

		template<>
		struct LuaEquivalent<unsigned int> : is_lua_Integer {};

		/// Forward declaration of templated way to push values into Lua
		template<typename T>
		struct ToLua;

		/// We know how to push lua_Integer
		template<>
		struct ToLua<lua_Integer> {
			static void push(lua_State * L, lua_Integer const val) {
				lua_pushinteger(L, val);
			}
		};

		/// We know how to push lua_Number
		template<>
		struct ToLua<lua_Number> {
			static void push(lua_State * L, lua_Number const val) {
				lua_pushnumber(L, val);
			}
		};

		/// We know how to push bool
		template<>
		struct ToLua<bool> {
			static void push(lua_State * L, bool const val) {
				lua_pushboolean(L, val);
			}
		};

		/// We know how to push const char *
		template<>
		struct ToLua<const char*> {
			static void push(lua_State * L, const char* val) {
				lua_pushstring(L, val);
			}
		};

		/// We know how to push const std::string
		template<>
		struct ToLua<const std::string> {
			static void push(lua_State * L, const std::string & val) {
				lua_pushstring(L, val.c_str());
			}
		};

		/// Fallback templated case: push value like its lua equivalent.
		template<typename T>
		struct ToLua {
			static void push(lua_State *L, T const& val) {
				ToLua< typename LuaEquivalent<T>::type >::push(L, val);
			}
		};

		template<typename T>
		bool pushIfTypeIs(lua_State *L, const introspection::Value &original) {
			static const introspection::Type& testType =
			    introspection::Reflection::getType(extended_typeid<T>());
			if (original.getType() == testType) {
				ToLua<T>::push(L, introspection::getInstance<T>(original));
				return true;
			}
			return false;
		}

	}

	Value::Value(const introspection::Value &v) : _value(v) {

		static const introspection::Type &t_referenced =
		    introspection::Reflection::getType(extended_typeid<osg::Referenced>());

		const introspection::Type &type = v.getType();
		if (type.isDefined()) {
			if (type.isNonConstPointer()) {
				const introspection::Type &p_type = type.getPointedType();
				if (p_type.isDefined())
					if (p_type.isSubclassOf(t_referenced)) {
						_referenced =
						    introspection::variant_cast<osg::Referenced*>(_value);
					}
			}
		}
	}

	Value::~Value() {
	}

	introspection::Value getValue(lua_State *L, int index) {
		int top = lua_gettop(L);
		index = (index > 0) ? index : top + index + 1;

		if (lua_isuserdata(L, index)) {
			Value *v = Value::get(L, index);
			if (v) {
				return v->get();
			} else {
				luaL_error(L,
				           "userdata can not be used as osgLua::introspection::Value");
			}
		}

		if (lua_isnumber(L, index)) {
			lua_Integer vi = lua_tointeger(L, index);
			lua_Number  vf = lua_tonumber(L, index);

			if (((lua_Number)vi) == vf) {
				return introspection::Value(vi);
			}
			return introspection::Value(vf);
		}

		if (lua_isstring(L, index)) {
			return introspection::Value(
			           std::string(lua_tostring(L, index))
			       );
		}

		if (lua_isboolean(L, index)) {
			return introspection::Value((lua_toboolean(L, index) == 0) ? false : true);
		}

		luaL_error(L,
		           "Value at %d(%s) can not be used as osgLua::introspection::Value",
		           index, lua_typename(L, lua_type(L, index)));

		return introspection::Value();
	}




	void Value::push(lua_State *L, const introspection::Value &original) {
		if (detail::pushIfTypeIs<bool>(L, original)) {
			return;
		}

		if (detail::pushIfTypeIs<int>(L, original)) {
			return;
		}
		if (detail::pushIfTypeIs<unsigned int>(L, original)) {
			return;
		}
		if (detail::pushIfTypeIs<lua_Integer>(L, original)) {
			return;
		}

		if (detail::pushIfTypeIs<float>(L, original)) {
			return;
		}
		if (detail::pushIfTypeIs<double>(L, original)) {
			return;
		}
		if (detail::pushIfTypeIs<lua_Number>(L, original)) {
			return;
		}

		if (detail::pushIfTypeIs<const char*>(L, original)) {
			return;
		}
		if (detail::pushIfTypeIs<const std::string>(L, original)) {
			return;
		}

		// if not is a basic type...
		// create a userdata
		Value** data = (Value**) lua_newuserdata(L, sizeof(Value*));

		// create/get the metatable
		_getOrCreateMetatable(L, original.getType());
		lua_setmetatable(L, -2);

		Value *v = new Value(original); //lua's copy
		*data = v;
	}



	Value* Value::get(lua_State *L, int index) {
		int top = lua_gettop(L);
		index = (index > 0) ? index : top + index + 1;

		if (_hasOsgLuaValueMetatable(L, index)) {
			Value *v = _rawGet(L, index);
			lua_settop(L, top);
			return v;
		}

		lua_settop(L, top);
		return 0;
	}

	Value* Value::getRequired(lua_State *L, int index) {
		Value *a = Value::get(L, index);
		if (a == 0) {
			luaL_error(L, "%s:%d Expected a osgLua userdata but get %s",
			           __FILE__, __LINE__, lua_typename(L, lua_type(L, index))) ;
		}
		return a;
	}


	int Value::getTypeInfo(lua_State *L) {
		const introspection::Type *type = 0;
		Value *v = Value::get(L, 1);
		if (v) {
			type = &(v->getType());
		} else if (lua_isstring(L, 1)) {
			try {
				const introspection::Type &t =
				    introspection::Reflection::getType(lua_tostring(L, 1));
				type = &t;
			} catch (introspection::TypeNotFoundException&) {
				return 0;
			}
		}

		if (type) {
			pushTypeInfo(L, type);
			return 1;
		}

		return 0;
	}

	int Value::getTypes(lua_State *L) {
		const introspection::TypeMap &map =
		    introspection::Reflection::getTypes();
		int counter = 1;

		lua_newtable(L);

		for (introspection::TypeMap::const_iterator i = map.begin();
		        i != map.end(); ++i) {
			introspection::Type *type = i->second;
			if (!type->isDefined() || type->isPointer()) {
				continue;
			}
			lua_pushstring(L, type->getQualifiedName().c_str());
			lua_rawseti(L, -2, counter);
			counter++;

		}
		return 1;
	}

	int Value::createByName(lua_State *L) {
		int top = lua_gettop(L);
		std::string name(luaL_checkstring(L, 1));
		try {
			const introspection::Type &type =
			    introspection::Reflection::getType(name);

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
