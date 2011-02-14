/**	@file	binding_detail/BindOsgToLua.inl
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
#include <osgLua/LoadWrapper>
#include <osgLua/Value>


// Standard includes
#include <iostream>


namespace vrjLua {

void bindOsgToLua(LuaStatePtr state) {
#ifdef VERBOSE
	std::cerr << "Started binding OSG to Lua..." << std::flush << std::endl;
#endif
	osgLua::open(state.get());
	bool ret = true;

	try {
		ret = osgLua::loadWrapper(state.get(), "osg");
	} catch (std::exception & e) {
		std::cerr << "ERROR: Caught an exception trying to load osgwrappers : " << e.what() << std::endl;
		std::cerr << "Make sure you have the osgwrappers installed in their default location with respect to OSG itself." << std::endl;
		std::cerr << "Cannot continue, exiting!" << std::endl;
		std::terminate();
	}

	if (!ret) {
		std::cerr << "Failure return code, trying to load wrappers for osg!" << std::endl;
		lua_pushstring(state.get(), "Failure return code, trying to load wrappers for osg!");
		lua_error(state.get());
		std::terminate();
	}

	manuallyBindOsgHelpers(state);
}

}// end of vrjLua namespace
