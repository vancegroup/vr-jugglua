/**	@file
	@brief	implementation of convenience binding

	@date
	2009-2011

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

//          Copyright Iowa State University 2009-2011.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


// Local includes
#include "BindvrjLuaToLua.h"
#include <vrjugglua/VRJLuaOutput.h>
#include <vrjugglua/LuaPath.h>
#include <vrjugglua/AddToLuaPaths.h>
#include <vrjugglua/LuaInclude.h>

// Library/third-party includes
#include <luabind/luabind.hpp>

#include <osgDB/Registry>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>

// Standard includes
#ifdef VERBOSE
#include <iostream>
#endif

namespace vrjLua {
	using namespace luabind;

	static inline std::string getDirectoryPart(std::string p) {
		if (p.empty()) {
			return p;
		}
		if (osgDB::fileExists(p)) {
			p = osgDB::convertFileNameToUnixStyle(osgDB::getRealPath(p));
			if (osgDB::fileType(p) == osgDB::REGULAR_FILE) {
				p = osgDB::getFilePath(p);
			}
		}
		if (p[p.size() - 1] != '/') {
			p.push_back('/');
		}
		return p;
	}
	static void appendToModelSearchPath(std::string const& path) {
		std::string p = getDirectoryPart(path);
		osgDB::Registry::instance()->getDataFilePathList().push_back(p);
	}

	static void appendToLuaRequirePath(LuaStateRawPtr s, std::string const& path) {
		std::string p = getDirectoryPart(path);

		luabind::object package(luabind::globals(s)["package"]);
		LuaSearchPath searchpath(object_cast<std::string>(package["path"]));

		searchpath.extend(SearchDirectory(p));

		package["path"] = searchpath.toString();
	}

	static luabind::object getModelSearchPath(lua_State *L) {
		luabind::object searchPath(newtable(L));
		osgDB::FilePathList paths = osgDB::Registry::instance()->getDataFilePathList();
		for (unsigned int i = 0; i < paths.size(); ++i) {
			searchPath[i + 1] = paths[i];
		}
		return searchPath;
	}

	static std::string findInModelSearchPath(std::string const& fn) {
		return osgDB::findDataFile(fn);
	}

	void BindvrjLuaToLua(LuaStatePtr state) {
#ifdef VERBOSE
		std::cerr << "Registering vrjLua module functions with Lua..." << std::flush << std::endl;
#endif
		module(state.get(), "vrjLua") [
		    def("appendToModelSearchPath", &appendToModelSearchPath),
		    def("getModelSearchPath", &getModelSearchPath),
		    def("findInModelSearchPath", &findInModelSearchPath),
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
