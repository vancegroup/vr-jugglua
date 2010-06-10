/**	@file	LuaConsole.cpp
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
#include "LuaConsole.h"
#include "VRJLuaOutput.h"

// Library/third-party includes
#include <luabind/object.hpp>

// Standard includes
#include <iostream>
#include <stdexcept>

namespace vrjLua {

LuaConsole::LuaConsole() {
	// No constructor body
	// A new script context is automatically created
}

LuaConsole::LuaConsole(LuaScript const& script) :
		_script(script) {
	// No constructor body
	// Using existing (provided) script context
}

LuaConsole::~LuaConsole() {
}

bool LuaConsole::getRunBufFromLuaGlobal() {

	LuaStatePtr state = _script.getLuaState().lock();
	if (!state) {
		throw std::runtime_error("Could not get a valid lua state pointer!");
	}

	luabind::object runbufLua(luabind::globals(state.get())["runbuf"]);
	if (!runbufLua) {
		throw std::runtime_error("Could not get a lua global named runbuf!");
	}

	_runbuf = luabind::object_cast<boost::shared_ptr<SynchronizedRunBuffer> >(runbufLua);
	if (!_runbuf) {
		throw std::runtime_error("Could not get a valid run buffer pointer from lua!");
	}
	return true;
}

bool LuaConsole::createRunBuf() {
	boost::shared_ptr<SynchronizedRunBuffer> buf(new SynchronizedRunBuffer(_script));
	_runbuf = buf;
	return (_runbuf);
}

bool LuaConsole::isValid() const {
	return (_runbuf) && (_script.isValid());
}

bool LuaConsole::runFile(std::string const& fn) {
	if (!_runbuf) {
		return false;
	}
	return _runbuf->addFile(fn);
}

bool LuaConsole::runString(std::string const& str) {
	if (!_runbuf) {
		return false;
	}
	return _runbuf->addString(str);
}

StubConsole::StubConsole() :
		LuaConsole() {
	VRJLUA_MSG_START(dbgVRJLUA_CONSOLE, MSG_STATUS)
			<< "StubConsole instantiated."
			<< VRJLUA_MSG_END(dbgVRJLUA_CONSOLE, MSG_STATUS);
}

StubConsole::StubConsole(LuaScript const& script) :
		LuaConsole(script) {
	VRJLUA_MSG_START(dbgVRJLUA_CONSOLE, MSG_STATUS)
				<< "StubConsole instantiated with provided LuaScript."
				<< VRJLUA_MSG_END(dbgVRJLUA_CONSOLE, MSG_STATUS);
}

StubConsole::~StubConsole() {
	VRJLUA_MSG_START(dbgVRJLUA_CONSOLE, MSG_STATUS)
				<< "StubConsole being destroyed."
				<< VRJLUA_MSG_END(dbgVRJLUA_CONSOLE, MSG_STATUS);
}


bool StubConsole::threadLoop() {
	VRJLUA_MSG_START(dbgVRJLUA_CONSOLE, MSG_STATUS)
				<< "StubConsole::threadLoop called - returning immediately because we're a stub."
				<< VRJLUA_MSG_END(dbgVRJLUA_CONSOLE, MSG_STATUS);
	return true;
}

void StubConsole::stopThread() {
	VRJLUA_MSG_START(dbgVRJLUA_CONSOLE, MSG_STATUS)
				<< "StubConsole::stopThread called - no-op because we're a stub."
				<< VRJLUA_MSG_END(dbgVRJLUA_CONSOLE, MSG_STATUS);
}

void StubConsole::appendToDisplay(std::string const& message) {
	VRJLUA_MSG_START(dbgVRJLUA_CONSOLE, MSG_STATUS)
				<< "appendToDisplay: " << message
				<< VRJLUA_MSG_END(dbgVRJLUA_CONSOLE, MSG_STATUS);
}

void StubConsole::setTitle(std::string const& title) {
	VRJLUA_MSG_START(dbgVRJLUA_CONSOLE, MSG_STATUS)
				<< "StubConsole::setTitle called with title " << title
				<< VRJLUA_MSG_END(dbgVRJLUA_CONSOLE, MSG_STATUS);
}

} // end of vrjLua namespace
