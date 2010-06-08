/**	@file	LuaConsole.h
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
#ifndef INCLUDED_vrjugglua_LuaConsole_h
#define INCLUDED_vrjugglua_LuaConsole_h

// Local includes
#include <vrjugglua/VRJLua_C_Interface.h>
#include <vrjugglua/LuaScript.h>
#include <vrjugglua/BindRunBufferToLua.h>

// Library/third-party includes
// - none

// Standard includes
#include <string>

namespace vrjLua {

class LuaConsole {
	public:
		LuaConsole();
		~LuaConsole();
		bool runFile(const std::string & fn);
		bool runString(const std::string & str);

	protected:
		LuaScript _script;
		SynchronizedRunBuffer* _runbuf;
};

// -- inline implementations -- /

}// end of vrjLua namespace

#endif // INCLUDED_vrjugglua_LuaConsole_h
