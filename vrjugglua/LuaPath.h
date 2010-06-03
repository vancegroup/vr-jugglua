/**	@file	LuaPath.h
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
#ifndef INCLUDED_vrjugglua_LuaPath_h
#define INCLUDED_vrjugglua_LuaPath_h

// Local includes
#include <vrjugglua/LuaScript.h>

// Library/third-party includes
// - none

// Standard includes
#include <string>

namespace vrjLua {

class LuaPath {
	public:
		LuaPath();

		std::string getPathToLuaScript(const std::string & scriptfn) const;
		bool prependLuaRequirePath(LuaStatePtr state) const;

	protected:
		std::string _findFilePath(const std::string & fn, const std::string & startingAt);

		bool _setJugglerEnvironment() const;

		std::string _root;
		std::string _luadir;

};

// -- inline implementations -- /

}// end of vrjLua namespace


#endif // INCLUDED_vrjugglua_LuaPath_h
