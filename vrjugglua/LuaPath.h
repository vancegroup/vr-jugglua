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
#include <queue>

namespace vrjLua {

class LuaPath {
	public:
		~LuaPath();
		static LuaPath& instance(std::string const& arg0 = "", std::string const& vrjlua_base = "");

		bool findAppRoot(std::string const& fn);
		std::string const& getAppRoot() const;
		std::string const& getExeDir() const;
		std::string const& getInitialPath() const;
		std::string getPathToLuaScript(const std::string & scriptfn) const;

		void addLuaRequirePath(LuaStatePtr state, std::string const& dirEndingInSlash);
		void updateLuaRequirePath(LuaStatePtr state);
		

	protected:
		LuaPath();
		void _init(std::string const& arg0, std::string const& vrjlua_base);

		static std::string _findFilePath(std::vector<std::string> const& startingPlaces, std::string const& qualified);
		static std::string _findFilePath(std::string const& startingAt, std::string const& fn);

		void _populateSearchPathsVector(LuaStatePtr state);
		void _setLuaSearchPaths(LuaStatePtr state);
		bool _setJugglerEnvironment() const;

		std::string _initialPath;
		std::string _exeDir;
		std::string _root;
		std::string _luadir;
		std::string _appRoot;

		bool _foundJuggler;
		std::string _jugglerRoot;

		bool _valid;

		std::deque<std::string> _searchPaths;
};

// -- inline implementations -- /

}// end of vrjLua namespace


#endif // INCLUDED_vrjugglua_LuaPath_h
