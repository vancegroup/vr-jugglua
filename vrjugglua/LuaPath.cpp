/**	@file	LuaRunBuffer.cpp
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

// Internal Includes
#include "LuaPath.h"
#include "VRJLuaOutput.h"

#include "LuaIncludeFull.h"

// Library includes
#include <boost/filesystem.hpp>

#include <vpr/System.h>

// Standard includes
#include <fstream>
#include <sstream>

namespace fs = boost::filesystem;

namespace vrjLua {

LuaPath::LuaPath() {
	fs::path initialPath = fs::initial_path();

	try {
		_root = _findFilePath("share/vrjugglua/lua/StateMachine.lua", initialPath.string());
		_luadir = (fs::path(_root) / "share/vrjugglua/lua/").string();
	} catch (std::runtime_error & e) {
		VRJLUA_MSG_START(dbgVRJLUA, MSG_WARNING)
				<< "LuaPath: " << e.what()
				<< ", will retry with unqualified path"
				<< VRJLUA_MSG_END(dbgVRJLUA, MSG_WARNING);
		_root = "";
		_luadir = "";
	}
	if (_root.empty()) {
		_root = _findFilePath("StateMachine.lua", initialPath.string());
		_luadir = _root;
	}

	_setJugglerEnvironment();
}


std::string LuaPath::getPathToLuaScript(const std::string & scriptfn) const {
	return (fs::path(_luadir) / scriptfn).string();
}

bool LuaPath::prependLuaRequirePath(LuaStatePtr state) const {
	std::ostringstream scr;
	scr << "package.path = ";
	// string to append
	scr << '"';
	scr << "?;";
	scr << "?.lua;";
	scr << _luadir << "?;";
	scr << _luadir << "?.lua;";
	scr << '"';
	// concatenation and rest of line
	scr << " .. package.path";

	return luaL_dostring(state.get(), scr.str().c_str());
}

std::string LuaPath::_findFilePath(const std::string & fn, const std::string & startingAt) {

	// find the config file - recursively searching up.
	fs::path location = startingAt;
	fs::path filepath = fs::path(location / fn);
	unsigned int count = 0;
	for (unsigned int i = 0;
			i < 5 && !location.empty() && fs::exists(location) && !fs::exists(filepath);
			++i) {
		VRJLUA_MSG_START(dbgVRJLUA, MSG_STATUS)
				<< "LuaPath: Searching for file '" << fn << "' in " << location.string()
				<< VRJLUA_MSG_END(dbgVRJLUA, MSG_STATUS);

		// go up one
		location = location / "..";
		location.normalize();
		filepath = fs::path(location / fn);
	}

	if (!fs::exists(filepath)) {
		/// couldn't find it anywhere...
		throw std::runtime_error("Could not find file " + fn);
	}

	return location.normalize().string();
}

bool LuaPath::_setJugglerEnvironment() const {
#ifdef _WIN32
	/// Clear these environment variables so that Juggler figures itself out.
	bool ret = vpr::System::setenv("VJ_BASE_DIR", _root);
	ret = vpr::System::setenv("SNX_BASE_DIR", _root) && ret;
	return ret;
#else
	return true;
#endif
}

} // end of vrjLua namespace
