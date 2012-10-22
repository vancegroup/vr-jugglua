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

#include "Value_metamethods.h"
#include <osgLua/Value>

#include <osgLua/introspection/variant_cast>
#include <osgLua/introspection/Value>
#include <osgLua/introspection/Type>
#include <osgLua/introspection/Exceptions>

#include "LuaIncludeFull.h"

#include <string>                       // for string, operator<<
#include <sstream>

namespace osgLua {

	namespace value_metamethods {

		int tostring(lua_State *L) {
			Value *a = Value::getRequired(L, 1);

			lua_pushstring(L, a->get().toString().c_str());
			return 1;
		}

		int minimal_tostring(lua_State *L) {
			Value *a = Value::getRequired(L, 1);
			std::stringstream s;
			s << "osgLua::Value (" << a->get().getType().getQualifiedName() << ") " << a;
			lua_pushstring(L, s.str().c_str());
			return 1;
		}

		int eq(lua_State *L) {
			Value *a = Value::getRequired(L, 1);
			Value *b = Value::getRequired(L, 2);

			bool ret;
			try {
				ret = a->get() == b->get();
			} catch (introspection::Exception &e) {
				luaL_error(L, "[%s:%d] %s", __FILE__, __LINE__, e.what().c_str());
			}

			lua_pushboolean(L, ret);

			return 1;
		}

		int lt(lua_State *L) {
			Value *a = Value::getRequired(L, 1);

			Value *b = Value::getRequired(L, 2);
			bool ret;
			try {
				ret = a->get() < b->get();
			} catch (introspection::Exception &e) {
				luaL_error(L, "[%s:%d] %s", __FILE__, __LINE__, e.what().c_str());
			}

			lua_pushboolean(L, ret);

			return 1;
		}

		int le(lua_State *L) {
			Value *a = Value::getRequired(L, 1);

			Value *b = Value::getRequired(L, 2);
			bool ret;
			try {
				ret = a->get() <= b->get();
			} catch (introspection::Exception &e) {
				luaL_error(L, "[%s:%d] %s", __FILE__, __LINE__, e.what().c_str());
			}

			lua_pushboolean(L, ret);

			return 1;
		}

	} // end of value_metamethods namespace

	void registerValueComparisons(lua_State * L, Comparability const& c) {
		if (c.eq) {
			lua_pushcfunction(L, &value_metamethods::eq);
			lua_setfield(L, -2, "__eq");
		}
		if (c.lt) {
			lua_pushcfunction(L, &value_metamethods::lt);
			lua_setfield(L, -2, "__lt");
		}
		if (c.le) {
			lua_pushcfunction(L, &value_metamethods::le);
			lua_setfield(L, -2, "__le");
		}
	};
} // end of osgLua namespace
