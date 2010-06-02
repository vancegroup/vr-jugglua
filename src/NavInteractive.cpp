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


namespace vrjLua {
NavInteractive::NavInteractive() {
	_script.runFile("osgnav-testbed.lua");

}

NavInteractive::~NavInteractive() {

}

bool NavInteractive::runFile(const std::string & fn) {
	SynchronizedRunBuffer
}

bool NavInteractive::runString(const std::string & str) {

}

bool isRunning() {

}

} // end of vrjLua namespace
