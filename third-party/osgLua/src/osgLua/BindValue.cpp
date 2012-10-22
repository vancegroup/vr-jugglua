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
#include "Value_metamethods.h"
#include "LuaIncludeFull.h"
#include "Notify.h"

#include <osgLua/introspection/Type>


#include "RegisterMathMetamethods.h"

namespace osgLua {
	static const char OSGLUAVALUEMETATABLE[] = "__isOsgLuaValue";

	static int osgLuaTypeTag(lua_State *L) {
		lua_pushboolean(L, true);
		return 1;
	}

	bool Value::_hasOsgLuaValueMetatable(lua_State *L, int index) {
		int top = lua_gettop(L);
		index = (index > 0) ? index : top + index + 1;

		if (lua_isuserdata(L, index)) {
			lua_getmetatable(L, index);
			lua_pushstring(L, OSGLUAVALUEMETATABLE);
			lua_gettable(L, -2);
			if (lua_tocfunction(L, -1) == &osgLuaTypeTag) {
				lua_settop(L, top);
				return true;
			}
		}
		lua_settop(L, top);
		return false;
	}


	void Value::_getOrCreateMetatable(lua_State *L, introspection::Type const& t) {
		// create/get the metatable
		if (luaL_newmetatable(L, t.getQualifiedName().c_str())) {
			OSG_INFO << "First time pushing " << t.getQualifiedName() << " to Lua - creating metatable!" << std::endl;

			// tag this as an osgLua value
			lua_pushcfunction(L, osgLuaTypeTag);
			lua_setfield(L, -2, OSGLUAVALUEMETATABLE);

			lua_pushcfunction(L, Value::_gc);
			lua_setfield(L, -2, "__gc");

			lua_pushcfunction(L, Value::_index);
			lua_setfield(L, -2, "__index");

			lua_pushcfunction(L, Value::_newindex);
			lua_setfield(L, -2, "__newindex");

			if (t.getReaderWriter()) {
				/// If we know how to turn it into a string
				lua_pushcfunction(L, value_metamethods::tostring);
			} else {
				/// Well, at least we can say what its type is
				lua_pushcfunction(L, value_metamethods::minimal_tostring);
			}
			lua_setfield(L, -2, "__tostring");

			/// Push these coomparison methods. For matrices, we will overwrite
			/// these when registering the math metamethods
			lua_pushcfunction(L, &value_metamethods::eq);
			lua_setfield(L, -2, "__eq");

			lua_pushcfunction(L, &value_metamethods::lt);
			lua_setfield(L, -2, "__lt");

			lua_pushcfunction(L, &value_metamethods::le);
			lua_setfield(L, -2, "__le");

			/// Bind mathematically-inclined values specially
			registerMathMetamethods(L, t);
		}
	}


} // end of osgLua namespace
