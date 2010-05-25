/**	@file	BindOsgToLua.cpp
	@brief	implementation

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
#include "BindOsgToLua.h"

// Library/third-party includes
#include <osgLua/osgLua.h>

// Standard includes
#include <iostream>

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
