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
	osgLua::loadWrapper(state.get(), "osg");
	osgLua::loadWrapper(state.get(), "osgDB");
	osgLua::loadWrapper(state.get(), "osgUtil");
}

}// end of vrjLua namespace


#endif // INCLUDED_vrjugglua_BindOsgToLua_h
