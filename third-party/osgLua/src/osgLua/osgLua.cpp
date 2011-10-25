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

#include <osgLua/LoadWrapper>
#include <osgLua/Value>
#include "osgLua.h"
#include "ArrayPropertyProxy.h"
#include "loadWrapperLib.h"
#include "CustomConverters.h"
#include "LuaIncludeFull.h"

#include <iostream>
#include <cstring>
#include "Type.h"
#include <osgLua/Callback>


#include <osg/Vec3d> // FIXME!!! (this is for checkings)

#include <osgDB/DynamicLibrary>

typedef osgDB::DynamicLibrary osglib;

int unload_osgWrapper(lua_State *L) {
	osglib **ptr = (osglib**) lua_touserdata(L, 1);
	osglib *lib = *ptr;
	lib->unref();
	return 0;
}

void osgLua::open(lua_State *L) {
	int top = lua_gettop(L);
	lua_getfield(L, LUA_REGISTRYINDEX, "osgLuaLibs");
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1); // remove nil
		lua_newtable(L); // create a table
		lua_pushvalue(L, -1); // copy it
		lua_setfield(L, LUA_REGISTRYINDEX, "osgLuaLibs"); // one copy to reg.

		lua_newtable(L);
		lua_pushcfunction(L, osgLua::Value::getTypes);
		lua_setfield(L, -2,  "getTypes");
		lua_pushcfunction(L, osgLua::Value::getTypeInfo);
		lua_setfield(L, -2, "getTypeInfo");
		lua_pushcfunction(L, osgLua::Value::createByName);
		lua_setfield(L, -2, "createByName");
		lua_pushcfunction(L, osgLua::lua_loadWrapper);
		lua_setfield(L, -2, "loadWrapper");
		lua_pushcfunction(L, osgLua::lua_loadObjectFile);
		lua_setfield(L, -2, "loadObjectFile");
		lua_pushcfunction(L, osgLua::lua_saveObjectFile);
		lua_setfield(L, -2, "saveObjectFile");
		lua_pushcfunction(L, osgLua::NodeCallback::createFromLua);
		lua_setfield(L, -2, "NodeCallback");

		lua_setglobal(L, "osgLua");
	}

	lua_settop(L, top);
}

void osgLua::get(lua_State *L) {
	lua_getfield(L, LUA_REGISTRYINDEX, "osgLuaLibs");
	if (lua_isnil(L, -1)) {
		osgLua::open(L);
		lua_getfield(L, LUA_REGISTRYINDEX, "osgLuaLibs");
	}
}

bool osgLua::loadWrapper(lua_State *L, const char *name) {
	int top = lua_gettop(L);

	//get or create osgLuaLibs in the registry
	osgLua::get(L);
	int osgLuaLibs = lua_gettop(L);

	// check if the library exists, if not try load it
	lua_getfield(L, osgLuaLibs , name);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		osglib * lib;
		try {
			lib = loadWrapperLib(name);
		} catch (std::exception & e) {
			std::cout << "Failed to load wrapper for " << name << " : " << e.what() << std::endl;
			return false;
		}

		/***********************************************************/
		/* REMOVE THIS IN THE FUTURE!!! (when solved)              */
		/***********************************************************/
		if (std::strcmp(name, "osg") == 0) {

			try {
				const std::type_info &tcheck =
				    introspection::Reflection::getType("osg::Vec3d").
				    getStdTypeInfo();

				if (tcheck != typeid(osg::Vec3d)) {
					std::cerr <<
					          "\n **********************************************************"
					          "\n ****************     W A R N I N G     *******************"
					          "\n **********************************************************"
					          "\n * osgWrapper was not correctly  loaded. osgIntrospection *"
					          "\n * requires in unix systems to be loaded with RTLD_GLOBAL *"
					          "\n * flag enabled, but  currently lua standard distribution *"
					          "\n * doesn't use it.                                        *"
					          "\n *                                                        *"
					          "\n * You can use osgLua interpreter (in osgLua/examples) it *"
					          "\n * is  the standard  lua interpreter  linked with osgLua, *"
					          "\n * that sovles the problem.                               *"
					          "\n *                                                        *"
					          "\n * If you have lua embedded  in your application you will *"
					          "\n * need to patch your lua distribution:                   *"
					          "\n * Search in  loadlib.c the line:                         *"
					          "\n *        void *lib = dlopen(path, RTLD_NOW);             *"
					          "\n * and replace it with:                                   *"
					          "\n *        void *lib = dlopen(path, RTLD_NOW|RTLD_GLOBAL); *"
					          "\n *                                                        *"
					          "\n * We expect lua developers solve this issue some time in *"
					          "\n * the future.                                            *"
					          "\n **********************************************************"
					          << std::endl;
					return false;
				}
			} catch (introspection::Exception & e) {
				std::cerr << "Could not get type info for osg::Vec3d in " << __FILE__ << ":" << __LINE__ << " - exception: " << e.what() << std::endl;
				return false;
			}
		}
		/***********************************************************/
		/***********************************************************/
		/***********************************************************/

		if (std::strcmp(name, "osg") == 0) {
			/// For the OSG main library, must register converters.
			registerCustomConverters();
		}
		osglib **ptr = reinterpret_cast<osglib**>(
		                   lua_newuserdata(L, sizeof(osglib*)));

		if (luaL_newmetatable(L, "osgLuaDynamicLibraries")) {
			// add a destructor handler
			lua_pushcfunction(L, unload_osgWrapper);
			lua_setfield(L, -2, "__gc");
		}
		lua_setmetatable(L, -2);
		lua_setfield(L, osgLuaLibs, name);

		// assing data and make a ref
		*ptr = lib;
		lib->ref();
	} else {
		// the library was loaded...
		lua_pop(L, 1);
	}

	// enable the namespace
	osgLua::Type::push(L, name);
	lua_setglobal(L, name);

	lua_settop(L, top);
	return true;
}

int osgLua::lua_loadWrapper(lua_State *L) {
	const char *name = lua_tostring(L, 1);
	luaL_argcheck(L, name != 0, 1, "need a string");
	if (!osgLua::loadWrapper(L, name)) {
		luaL_error(L, "Can not load osg wrapper for %s", name);
	}
	return 0;
}

int luaopen_osgLua(lua_State *L) {
	osgLua::open(L);

	return 0;
}

int luaopen_libosgLua(lua_State *L) {
	return luaopen_osgLua(L);
}
