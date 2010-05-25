/**	@file	BindOsgToLua.h
	@brief	header

	@date
	2009-2010

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/
#pragma once
#ifndef INCLUDED_vrjugglua_BindOsgToLua_h
#define INCLUDED_vrjugglua_BindOsgToLua_h

// Local includes
#include "LuaScript.h"

// Library/third-party includes
#include <osgLua/osgLua.h>

// Standard includes
// - none

namespace vrjLua {

void bindOsgToLua(LuaStatePtr state) {
	luaopen_osgLua(state.get());
	bool ret;

	ret = osgLua::loadWrapper(state.get(), "osg");
	if (!ret) {
		std::cerr << "Could not load Lua wrapper for osg!" << std::endl;
		return;
	}

	ret = osgLua::loadWrapper(state.get(), "osgDB");
	if (!ret) {
		std::cerr << "Could not load Lua wrapper for osgDB!" << std::endl;
		return;
	}

	ret = osgLua::loadWrapper(state.get(), "osgUtil");
	if (!ret) {
		std::cerr << "Could not load Lua wrapper for osgUtil!" << std::endl;
		return;
	}
}

}// end of vrjLua namespace


#endif // INCLUDED_vrjugglua_BindOsgToLua_h
