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
#include <vector>

namespace vrjLua {

class LuaPath {
	public:
		static LuaPath& instance(std::string const& arg0 = "", std::string const& vrjlua_base = "");

		bool findAppRoot(std::string const& fn);
		std::string const& getAppRoot() const;
		std::string getPathToLuaScript(const std::string & scriptfn) const;
		bool prependLuaRequirePath(LuaStatePtr state) const;



	protected:
		LuaPath();
		void _init(std::string const& arg0, std::string const& vrjlua_base);

		static std::string _findFilePath(std::vector<std::string> const& startingPlaces, std::string const& qualified);
		static std::string _findFilePath(std::string const& startingAt, std::string const& fn);

		bool _setJugglerEnvironment() const;

		std::string _root;
		std::string _luadir;
		std::string _appRoot;

		bool _foundJuggler;
		std::string _jugglerRoot;

		bool _valid;
};

// -- inline implementations -- /

}// end of vrjLua namespace


#endif // INCLUDED_vrjugglua_LuaPath_h
