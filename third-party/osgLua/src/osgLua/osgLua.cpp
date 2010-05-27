#include "osgLua.h"

#include <iostream>
#include "Value.h"
#include "Type.h"
#include <osgLua/Callback>

#include <osg/Version>

#include <osg/Vec3> // FIXME!!! (this is for checkings)

#include <osgDB/DynamicLibrary>
#include <osgDB/ReadFile>

std::string getLibraryNamePrepend() {
	return std::string("osgPlugins-")+std::string(osgGetVersion())+std::string("/");
}

// borrowed from osgDB...
std::string createLibraryNameForWrapper(const std::string& ext)
{

#if defined(WIN32)
    // !! recheck evolving Cygwin DLL extension naming protocols !! NHV
    #ifdef __CYGWIN__
        return "cygosgwrapper_"+ext+".dll";
    #elif defined(__MINGW32__)
        return "libosgwrapper_"+ext+".dll";
    #else
        #ifdef _DEBUG
            return "osgwrapper_"+ext+"d.dll";
        #else
            return "osgwrapper_"+ext+".dll";
        #endif
    #endif
#elif macintosh
    return "osgwrapper_"+ext;
#elif defined(__hpux__)
    // why don't we use PLUGIN_EXT from the makefiles here?
    return "osgwrapper_"+ext+".sl";
#else
    return "osgwrapper_"+ext+".so";
#endif
}

typedef osgDB::DynamicLibrary osglib;

int unload_osgWrapper(lua_State *L)
{
	osglib **ptr = (osglib**) lua_touserdata(L,1);
	osglib *lib = *ptr;
	lib->unref();
	return 0;
}

bool osgLua::loadWrapper(lua_State *L, const char *name) {
	int top = lua_gettop(L);

	//get or create osgLuaLibs in the registry
	lua_getfield(L, LUA_REGISTRYINDEX, "osgLuaLibs" );
	if (lua_isnil(L,-1))
	{
		lua_pop(L,1); // remove nil
		lua_newtable(L); // create a table
		lua_pushvalue(L,-1); // copy it
		lua_setfield(L, LUA_REGISTRYINDEX, "osgLuaLibs"); // one copy to reg.
	}
	int osgLuaLibs = lua_gettop(L);

	// check if the library exists, if not try load it
	lua_getfield(L, osgLuaLibs , name);
	if (lua_isnil(L,-1))
	{
		lua_pop(L,1);
		std::string fn = createLibraryNameForWrapper(name);
		osglib *lib = osgDB::DynamicLibrary::loadLibrary( fn );

		// try it with an extension
		if (!lib) {
			fn = getLibraryNamePrepend() + createLibraryNameForWrapper(name);
			lib = osgDB::DynamicLibrary::loadLibrary(fn);
		}

#ifdef VERBOSE
		if (lib) {
			std::cerr << "Successfully loaded " << fn << std::endl;
		}
#endif

		if (!lib)
		{
			std::cerr << "Failed following osgDB::DynamicLibrary::loadLibrary call: trying to load "<< fn << std::endl;
			return false;
		}

		/***********************************************************/
		/* REMOVE THIS IN THE FUTURE!!! (when solved)              */
		/***********************************************************/
		if (strcmp(name,"osg") == 0)
		{
			const std::type_info &tcheck =
				osgIntrospection::Reflection::getType("osg::Vec3").
				getStdTypeInfo();
			if (tcheck != typeid(osg::Vec3))
			{
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
		}
		/***********************************************************/
		/***********************************************************/
		/***********************************************************/

		osglib **ptr = reinterpret_cast<osglib**>(
			lua_newuserdata(L, sizeof(osglib*)) );

		if (luaL_newmetatable(L, "osgLuaDynamicLibraries"))
		{
			// add a destructor handler
			lua_pushcfunction(L, unload_osgWrapper);
			lua_setfield(L, -2, "__gc");
		}
		lua_setmetatable(L, -2);
		lua_setfield(L, osgLuaLibs, name);

		// assing data and make a ref
		*ptr = lib;
		lib->ref();
	}
	else
	{
		// the library was loaded...
		lua_pop(L,1);
	}

	// enable the namespace
	osgLua::Type::push(L,name);
	lua_setglobal(L,name);

	lua_settop(L, top);
	return true;
}

int osgLua::lua_loadWrapper(lua_State *L)
{
	const char *name = lua_tostring(L,1);
	luaL_argcheck(L, name != 0, 1, "need a string");
	if(!osgLua::loadWrapper(L, name))
	{
		luaL_error(L, "Can not load osg wrapper for %s", name);
	}
	return 0;
}

int osgLua::lua_loadObjectFile(lua_State *L)
{
	const char *name = lua_tostring(L,1);
	luaL_argcheck(L, name != 0, 1, "need a string");
	osg::Object *obj = osgDB::readObjectFile(name);
	osgLua::Value::push(L, obj);
	return 1;
}

int luaopen_osgLua(lua_State *L)
{

	lua_newtable(L);
	lua_pushcfunction(L,osgLua::Value::getTypes);
	lua_setfield(L,-2,  "getTypes");
	lua_pushcfunction(L,osgLua::Value::getTypeInfo);
	lua_setfield(L,-2, "getTypeInfo");
	lua_pushcfunction(L,osgLua::Value::createByName);
	lua_setfield(L,-2, "createByName");
	lua_pushcfunction(L, osgLua::lua_loadWrapper );
	lua_setfield(L,-2, "loadWrapper");
	lua_pushcfunction(L, osgLua::lua_loadObjectFile );
	lua_setfield(L,-2, "loadObjectFile");
	lua_pushcfunction(L, osgLua::NodeCallback::createFromLua );
	lua_setfield(L,-2, "NodeCallback");

	lua_setglobal(L, "osgLua");
	return 0;
}

int luaopen_libosgLua(lua_State *L)
{
	return luaopen_osgLua(L);
}
