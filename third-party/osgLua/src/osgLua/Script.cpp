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


#include "InternalConfig.h"
#include <osgLua/Script>
#include <osgLua/Value>
#include <osgLua/LoadWrapper>
#include "osgLua.h"
#include "Type.h"
#include "LuaIncludeFull.h"

#include <osgLua/introspection/Value>
#include <osgLua/introspection/Type>

namespace osgLua {

	Script::Script(bool defaultLibs) : L(0) {
		L = luaL_newstate();
		if (defaultLibs) {
			luaL_openlibs(L);
		}
		luaopen_osgLua(L);
		lua_pushlightuserdata(L, (void*)this);
		lua_setfield(L, LUA_REGISTRYINDEX, "Script_this");
	}

	Script::~Script() {
		lua_close(L);
		L = 0;
	}

	void Script::loadFile(const char *filename, introspection::ValueList *vl) {
		execute(luaL_loadfile(L, filename) , vl);
	}

	void Script::execute(const std::string &code, introspection::ValueList *vl,
	                     const std::string &aux) {
		execute(
		    luaL_loadbuffer(L, code.c_str(), code.length(), aux.c_str()), vl
		);
	}

	bool Script::call(const std::string &funcName, const introspection::ValueList *params,
	                  introspection::ValueList *results) {
		bool result = true;
		int top = lua_gettop(L);

		//1st get the function to call
		lua_getglobal(L, funcName.c_str());
		if (lua_isnil(L, -1)) {
			// the function doesn't exist
			result = false;
		} else { // the function exists
			int num_params = 0;
			//2nd push the parameters
			if (params) {
				for (introspection::ValueList::const_iterator i =
				            params->begin(); i != params->end(); ++i) {
					Value::push(L, (*i));
					++num_params;
				}
			}
			//3rd Call the function
			if (lua_pcall(L, num_params, LUA_MULTRET, 0)) {
				Exception e(lua_tostring(L, -1));
				lua_settop(L, top);
				throw e;
			}
			//4th Collect the results
			if (results) {
				for (int i = top + 1; i < lua_gettop(L); ++i) {
					results->push_back(Value::get(L, i));
				}
			}
		}
		// restore the stack
		lua_settop(L, top);
		return result; // return true or false
	}

	void Script::execute(int value, introspection::ValueList *vl) {
		int top = lua_gettop(L) - 1;
		if (value) {
			Exception e(lua_tostring(L, -1));
			lua_settop(L, top);
			throw e;
		}
		if (lua_pcall(L, 0, LUA_MULTRET, 0)) {
			Exception e(lua_tostring(L, -1));
			lua_settop(L, top);
			throw e;
		}
		if (vl) {
			for (int i = top + 1; i < lua_gettop(L); ++i) {
				vl->push_back(Value::get(L, i));
			}
		}
		lua_settop(L, top);
	}

	bool Script::enable(const std::string &namespaceName) {
		return loadWrapper(L, namespaceName.c_str());
	}

	void Script::set(const introspection::Value &val,
	                 const std::string &name) {
		Value::push(L, val);
		lua_setglobal(L, name.c_str());
	}

	introspection::Value Script::get(const std::string &name) {
		int top = lua_gettop(L);
		introspection::Value v;
		lua_getglobal(L, name.c_str());
		if (!lua_isnil(L, -1)) {
			v = getValue(L, -1);
		}
		lua_settop(L, top);
		return v;
	}

	Script* Script::getScript(lua_State *L) {
		lua_getfield(L, LUA_REGISTRYINDEX, "Script_this");
		Script *result = reinterpret_cast<Script*>(lua_touserdata(L, -1));
		lua_pop(L, 1);
		return result;
	}


} // end of osgLua namespace
