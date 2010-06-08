/**	@file	SynchronizedRunBuffer.h
	@brief	header

	@date
	2009-2010

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/
#pragma once
#ifndef INCLUDED_vrjugglua_SynchronizedRunBuffer_h
#define INCLUDED_vrjugglua_SynchronizedRunBuffer_h

// Local includes
#include "LuaScript.h"
#include "LuaRunBuffer.h"

// Library/third-party includes
#include <plugins/ApplicationDataManager/UserData.h>

#include <luabind/object.hpp>

// Standard includes
// - none

namespace vrjLua {

class SynchronizedRunBuffer {
	public:
		SynchronizedRunBuffer(luabind::object const& delegate);
		SynchronizedRunBuffer(LuaStatePtr const& state);
		SynchronizedRunBuffer(LuaScript const& script);

		bool addFile(std::string const& filename, bool blocking = false);
		bool addString(std::string const& str, bool blocking = false);
		unsigned int runBuffer();
	protected:
		cluster::UserData<LuaRunBuffer> _runBuf;
};

} // end of vrjLua namespace



#endif // INCLUDED_vrjugglua_SynchronizedRunBuffer_h
