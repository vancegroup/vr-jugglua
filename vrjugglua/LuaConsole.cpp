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

// Library/third-party includes


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
}

bool LuaConsole::createRunBuf() {
	boost::shared_ptr<SynchronizedRunBuffer> buf(new SynchronizedRunBuffer(_script));
	_runbuf = buf;
	return (_runbuf);
}

void LuaConsole::setExitCallback(boost::function<void ()> callback) {
	_exitCallback = callback;
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

void LuaConsole::_signalThreadExit() {
	if (_exitCallback) {
		_exitCallback();
	}
}

} // end of vrjLua namespace
