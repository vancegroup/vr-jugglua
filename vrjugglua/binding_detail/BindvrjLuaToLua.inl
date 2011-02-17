/**	@file	binding_detail/BindvrjLuaToLua.inl
	@brief	implementation of convenience binding

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
#include "BindvrjLuaToLua.h"
#include <vrjugglua/VRJLuaOutput.h>
#include <vrjugglua/LuaPath.h>

// Library/third-party includes
#include <luabind/luabind.hpp>

#include <osgDB/Registry>

// Standard includes
#ifdef VERBOSE
#include <iostream>
#endif

namespace vrjLua {
using namespace luabind;

static void appendToModelSearchPath(std::string const& path) {
	osgDB::Registry::instance()->getDataFilePathList().push_back(path);
}

static void appendToLuaRequirePath(LuaStateRawPtr s, std::string const& path) {
	LuaPath::instance().addLuaRequirePath(borrowStatePtr(s), path);
}

static luabind::object getModelSearchPath(lua_State *L) {
	luabind::object searchPath(newtable(L));
	osgDB::FilePathList paths = osgDB::Registry::instance()->getDataFilePathList();
	for (unsigned int i = 0; i < paths.size(); ++i) {
		searchPath[i+1] = paths[i];
	}
	return searchPath;
}

void BindvrjLuaToLua(LuaStatePtr state) {
#ifdef VERBOSE
	std::cerr << "Registering vrjLua module functions with Lua..." << std::flush << std::endl;
#endif
	module(state.get(), "vrjLua") [
		def("appendToModelSearchPath", &appendToModelSearchPath),
		def("getModelSearchPath", &getModelSearchPath),
		def("appendToLuaRequirePath", &appendToLuaRequirePath),
		def("safePrint", &LuaScript::doPrint)
	];
	
	std::string const& shareDir = LuaPath::instance().getShareDir();
	if (!shareDir.empty()) {
		std::cout << "Found VR JuggLua share directory (" << shareDir << "), adding to model search path..." << std::endl;
		appendToModelSearchPath(shareDir);
	}

	std::string const& rootDir = LuaPath::instance().getRootDir();
	if (!rootDir.empty()) {
		std::cout << "Found root directory (" << rootDir << "), adding to model search path..." << std::endl;
		appendToModelSearchPath(rootDir);
		
		std::cout << "Adding config directory (" << rootDir << "/etc/vrjugglua/) to Lua package path..." << std::endl;
		appendToLuaRequirePath(state.get(), rootDir + "/etc/vrjugglua/");
	}


}

}// end of vrjLua namespace
