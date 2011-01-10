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

#include <boost/algorithm/string/replace.hpp>

#include <vrj/Kernel/Kernel.h>

// Standard includes
#include <iostream>
#include <stdexcept>

namespace vrjLua {

/// Initialize static member
LuaConsole * LuaConsole::s_console = NULL;

LuaConsole * LuaConsole::getConsole() {
	return s_console;
}

static void consolePrintFunction(std::string const& str) {
	LuaConsole * ptr = LuaConsole::getConsole();
	if (ptr) {
		std::string modified = str;
		boost::algorithm::replace_all(modified, "\n", "\n-- ");
		ptr->appendToDisplay("-- " + modified);
	}
}

LuaConsole::LuaConsole() :
	_loggingActive(false) {
#ifdef VERBOSE
	std::cout << "In constructor " << __FUNCTION__ << " at " << __FILE__ << ":" << __LINE__ << " with this=" << this << std::endl;
#endif
	s_console = this;
	_script.setPrintFunction(consolePrintFunction);
	// A new script context is automatically created
}

LuaConsole::LuaConsole(LuaScript const& script) :
		_loggingActive(false),
		_script(script) {
#ifdef VERBOSE
	std::cout << "In constructor " << __FUNCTION__ << " at " << __FILE__ << ":" << __LINE__ << " with this=" << this << std::endl;
#endif
	s_console = this;
	_script.setPrintFunction(consolePrintFunction);
	// Using existing (provided) script context
}

LuaConsole::~LuaConsole() {
#ifdef VERBOSE
	std::cout << "In destructor " << __FUNCTION__ << " at " << __FILE__ << ":" << __LINE__ << " with this=" << this << std::endl;
#endif
	if (s_console == this) {
		s_console = NULL;
		_script.setPrintFunction(boost::function<void (std::string const&)>());
	}
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

	/// @todo fix global
	/*
	LuaStatePtr state = _script.getLuaState().lock();
	if (!state) {
		throw std::runtime_error("Could not get a valid lua state pointer!");
	}

	/// Set the global "runbuf" variable
	luabind::globals(state.get())["runbuf"] = buf;
	*/
	return (_runbuf);
}

bool LuaConsole::isValid() const {
	return (_runbuf) && (_script.isValid());
}

bool LuaConsole::addFile(std::string const& fn) {
	if (!_runbuf) {
		throw std::runtime_error("Could not get the run buffer to add a file to!");
	}
	std::string code;
	code = "dofile('";
	code += fn;
	code += "')\n";

	bool ret = addString(code);

	return ret;
}

bool LuaConsole::addString(std::string const& str) {
	if (!_runbuf) {
		throw std::runtime_error("Could not get the run buffer to add a string to!");
	}
	bool ret = _runbuf->addString(str);
	std::string code;
	if (ret) {
		code = str + "\n";
	} else {
		code = "-- Failed attempting to add the following code to the buffer:\n";
		code += "--[[\n";
		code += str;
		code += "\n]]--\n";
	}
	appendToDisplay(code);
	return ret;
}

void LuaConsole::captureStdOut() {
	if (this->supportsAlternateLogging()) {
		_loggingActive = true;
		std::cout.rdbuf(_log.rdbuf());
	}
}

void LuaConsole::captureStdErr() {
	if (this->supportsAlternateLogging()) {
		_loggingActive = true;
		std::cout.rdbuf(_log.rdbuf());
	}
}

bool LuaConsole::runFileImmediately(std::string const& fn) {
	return _script.runFile(fn);
}

bool LuaConsole::runStringImmediately(std::string const& str) {
	return _script.runString(str);
}

bool LuaConsole::requireModuleImmediately(std::string const& module) {
	return _script.requireModule(module);
}

bool LuaConsole::runBuffer() {
	if (!_runbuf) {
		return false;
	}

#ifdef VERBOSE
	unsigned int ret = _runbuf->runBuffer();
	if (ret > 0) {
		std::cerr << "LuaConsole: ran " << ret << " entries successfully." << std::endl;
	}
#else
	_runbuf->runBuffer();
#endif
	return true;
}

LuaScript& LuaConsole::getScript() {
	return _script;
}


void StubConsole::setup(int & argc, char * argv[]) {
	VRJLUA_MSG_START(dbgVRJLUA_CONSOLE, MSG_STATUS)
		<< "StubConsole::setup called with these arguments:"
		<< VRJLUA_MSG_END(dbgVRJLUA_CONSOLE, MSG_STATUS);
	for (int i = 0; i < argc; ++i) {
		VRJLUA_MSG_START(dbgVRJLUA_CONSOLE, MSG_STATUS)
			<< "   '" << argv[i] << "'"
			<< VRJLUA_MSG_END(dbgVRJLUA_CONSOLE, MSG_STATUS);
	}
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
	vrj::Kernel::instance()->waitForKernelStop();
}

bool StubConsole::threadLoop() {
	VRJLUA_MSG_START(dbgVRJLUA_CONSOLE, MSG_STATUS)
				<< "StubConsole::threadLoop called - joining the kernel thread because we're a stub."
				<< VRJLUA_MSG_END(dbgVRJLUA_CONSOLE, MSG_STATUS);
	vrj::Kernel::instance()->waitForKernelStop();
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
