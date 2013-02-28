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


#include "InternalConfig.h"

#include <osgLua/Callback>
#include <osgLua/Value>
#include <osgLua/introspection/Value>

#include "LuaIncludeFull.h"


namespace osgLua {

	Callback::Callback(Script *s, const char *name) : _script(s), _ref(0), _L(0) {
		lua_State *L = _script->getLuaState();
		if (name) {
			lua_getglobal(L, name);
		}
		init(L);
	}

	Callback::Callback(lua_State *L) : _script(0), _ref(0), _L(0) {
		init(L);
	}

	void Callback::init(lua_State *Luita) {
		_L = Luita;
		// do I have a function in the top of the stack ?
		if (lua_isfunction(_L, -1)) {
			// get/create callbacks table
			lua_getfield(_L, LUA_REGISTRYINDEX, "callbacks");
			if (!lua_istable(_L, -1)) {
				lua_pop(_L, 1);
				lua_newtable(_L);
				lua_pushvalue(_L, -1); // leave a copy of the table in the stack
				lua_setfield(_L, LUA_REGISTRYINDEX, "callbacks");
			}
			lua_pushvalue(_L, -2);  // copy the function
			_ref = luaL_ref(_L, -2); // pops the function and create a ref
			lua_pop(_L, 1); // remove the original function
		} else {
			lua_pop(_L, 1); // remove the function
			throw ::osgLua::Exception(
			    "Callback not initialized correctly with a function");
		}
	}

	Callback::~Callback() {
		lua_getfield(_L, LUA_REGISTRYINDEX, "callbacks");
		luaL_unref(_L, -1, _ref);
		lua_pop(_L, 1);
	}

	inline void Callback::pushFunc() {
		lua_getfield(_L, LUA_REGISTRYINDEX, "callbacks");
		lua_rawgeti(_L, -1, _ref);
	}

	inline void Callback::call(int num) {
		if (lua_pcall(_L, num, 0, 0)) {
			if (_script.valid()) {
				osgLua::Exception e(lua_tostring(_L, -1));
				lua_pop(_L, 1); // the error...
				throw e;
			}
			luaL_error(_L, lua_tostring(_L, -1));
		}
	}

	void Callback::execute() {
		pushFunc();
		call(0);
	}

	void Callback::execute(P p1) {
		pushFunc();
		Value::push(_L, p1);
		call(1);
	}

	void Callback::execute(P p1, P p2) {
		pushFunc();
		Value::push(_L, p1);
		Value::push(_L, p2);
		call(2);
	}

	void Callback::execute(P p1 , P p2 , P p3) {
		pushFunc();
		Value::push(_L, p1);
		Value::push(_L, p2);
		Value::push(_L, p3);
		call(3);
	}

	void Callback::execute(P p1, P p2 , P p3, P p4) {
		pushFunc();
		Value::push(_L, p1);
		Value::push(_L, p2);
		Value::push(_L, p3);
		Value::push(_L, p4);
		call(4);
	}

	int NodeCallback::createFromLua(lua_State *L) {
		luaL_argcheck(L, lua_isfunction(L, 1), 1, "Expected a function...");
		Script *s = Script::getScript(L);
		NodeCallback *callback =
		    (s == 0) ? new NodeCallback(L) : new NodeCallback(s, 0) ;
		Value::push(L, (osg::NodeCallback*) callback);
		return 1;
	}
}
