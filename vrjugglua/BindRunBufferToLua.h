/**	@file	BindRunBufferToLua.h
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
#ifndef INCLUDED_vrjugglua_BindRunBufferToLua_h
#define INCLUDED_vrjugglua_BindRunBufferToLua_h

// Local includes
#include "LuaScript.h"
#include "LuaRunBuffer.h"

// Library/third-party includes
#include <plugins/ApplicationDataManager/UserData.h>

#include <luabind/object.hpp>

// Standard includes
// - none

namespace vrjLua {

void bindRunBufferToLua(LuaStatePtr state);

class SynchronizedRunBuffer {
	public:
		SynchronizedRunBuffer(luabind::object const& delegate);
		bool addFile(const std::string & filename, bool blocking = false);
		bool addString(const std::string & str, bool blocking = false);
		unsigned int runBuffer();
	protected:
		cluster::UserData<LuaRunBuffer> _runBuf;
};

} // end of vrjLua namespace



#endif // INCLUDED_vrjugglua_BindRunBufferToLua_h
