/**	@file	binding_detail/BindOsgToLua.cpp
	@brief	implementation of binding using osgLua

	@date
	2009-2010

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

// Local includes
#ifndef LUABIND_COMBINED_COMPILE
#include <vrjugglua/osgLuaBind.h>
#endif
#include "BindOsgToLua.h"
#include "OsgHelpers.h"
#include "../LuaIncludeFull.h"

// Library/third-party includes
#include <osgLua/osgLua.h>
#include <osgLua/Value.h>


// Standard includes
#include <iostream>


namespace vrjLua {



void bindOsgToLua(LuaStatePtr state) {
#ifdef VERBOSE
	std::cerr << "Started binding OSG to Lua..." << std::flush << std::endl;
#endif
	luaopen_osgLua(state.get());
#ifdef VERBOSE
	luaL_dostring(state.get(), "print('osgLua: ' .. tostring(osgLua.getTypes()))");
#endif
	bool ret;

#ifdef AUTOLOAD_WRAPPER_OSG
	ret = osgLua::loadWrapper(state.get(), "osg");
	if (!ret) {
		std::cerr << "Could not load Lua wrapper for osg!" << std::endl;
		lua_pushstring(state.get(), "Could not load Lua wrapper for osg!");
		lua_error(state.get());
		return;
	}
#endif

#ifdef AUTOLOAD_WRAPPER_OSGDB
	ret = osgLua::loadWrapper(state.get(), "osgDB");
	if (!ret) {
		std::cerr << "Could not load Lua wrapper for osgDB!" << std::endl;
		lua_pushstring(state.get(), "Could not load Lua wrapper for osgDB!");
		lua_error(state.get());
		return;
	}
#endif

#ifdef AUTOLOAD_WRAPPER_OSGUTIL
	ret = osgLua::loadWrapper(state.get(), "osgUtil");
	if (!ret) {
		std::cerr << "Could not load Lua wrapper for osgUtil!" << std::endl;
		lua_pushstring(state.get(), "Could not load Lua wrapper for osgUtil!");
		lua_error(state.get());
		return;
	}
#endif

	manuallyBindOsgHelpers(state);
}

}// end of vrjLua namespace
