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
#include "LuaRunBuffer.h"

// Library includes
#include <vpr/Sync/Guard.h>

// Standard includes
// - none

namespace vrjLua {
LuaRunBuffer::LuaRunBuffer() {
		
}

bool LuaRunBuffer::addFile(const std::string & fn, bool blocking = false) {
	vpr::Guard<vpr::Mutex> guard(_lock, blocking);
	if (guard.locked()) {
		
		
	} else {
		return false;
	}
}

bool LuaRunBuffer::addString(const std::string & str, bool blocking = false) {
	
}

int LuaRunBuffer::runBuffer(LuaScript & script) {
	
}

} // end of vrjLua namespace
