/**	@file	LuaScript.h
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
#ifndef INCLUDED_vrjugglua_LuaScript_h
#define INCLUDED_vrjugglua_LuaScript_h

// Local includes
// - none

// Library/third-party includes
extern "C"
{
    #include "lua.h"
}

#include <boost/shared_ptr.hpp>

// Standard includes
#include <string>

namespace vrjLua {

typedef boost::shared_ptr<lua_State> LuaStatePtr;

class LuaScript {
	public:
		LuaScript();

		/// @brief constructor from an externally-allocated state
		explicit LuaScript(lua_State * state);

		/// @brief copy constructor - doesn't re-bind
		LuaScript(const LuaScript & other);

		bool runFile(const std::string & fn);
		bool call(const std::string & func);

	protected:
		void _applyBindings();
		LuaStatePtr _state;
};

// -- inline implementations -- /

}// end of vrjLua namespace


#endif // INCLUDED_vrjugglua_LuaScript_h
