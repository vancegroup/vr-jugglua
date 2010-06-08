/**	@file	NavInteractive.cpp
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
#include "NavInteractive.h"

// Library/third-party includes
LUA_C_INTERFACE_BEGIN
#include "lauxlib.h"
#include "lualib.h"
LUA_C_INTERFACE_END

// Standard includes
#include <iostream>
#include <stdexcept>

namespace vrjLua {
NavInteractive::NavInteractive() :
		_runbuf(NULL) {
	bool ret = _script.requireModule("osgnav-testbed");
	if (!ret) {
		throw std::runtime_error("Could not find and run file osgnav-testbed.lua - make sure Lua can find it");
	}

	LuaStatePtr state = _script.getLuaState().lock();
	if (!state) {
		throw std::runtime_error("Could not get a valid lua state pointer!");
	}

	luabind::object runbufLua(luabind::globals(state.get())["runbuf"]);
	if (!runbufLua) {
		throw std::runtime_error("Could not get a lua global named runbuf!");
	}

	_runbuf = luabind::object_cast<SynchronizedRunBuffer*>(runbufLua);
	if (!_runbuf) {
		throw std::runtime_error("Could not get a valid run buffer pointer from lua!");
	}
}

NavInteractive::~NavInteractive() {

}

bool NavInteractive::runFile(const std::string & fn) {
	if (!_runbuf) {
		return false;
	}
	return _runbuf->addFile(fn);
}

bool NavInteractive::runString(const std::string & str) {
	if (!_runbuf) {
		return false;
	}
	return _runbuf->addString(str);
}

} // end of vrjLua namespace
