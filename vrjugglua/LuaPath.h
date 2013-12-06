/**	@file
        @brief	header

        @date
        2009-2011

        @author
        Ryan Pavlik
        <rpavlik@iastate.edu> and <abiryan@ryand.net>
        http://academic.cleardefinition.com/
        Iowa State University Virtual Reality Applications Center
        Human-Computer Interaction Graduate Program
*/

//          Copyright Iowa State University 2009-2011.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

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
#include <deque>
#include <iosfwd>

namespace vrjLua {

    class LuaPath {
      public:
        ~LuaPath();
        static LuaPath &instance(std::string const &arg0 = "",
                                 std::string const &vrjlua_base = "");

        std::string findFilePath(std::string const &fn);
        bool findAppRoot(std::string const &fn);
        std::string const &getAppRoot() const;
        std::string const &getRootDir() const;
        std::string const &getShareDir() const;
        std::string const &getExeDir() const;
        std::string const &getInitialPath() const;
        std::string const &getLuaDir() const;

        void debugDump(std::ostream &s) const;

        /// DEPRECATED
        void addLuaRequirePath(LuaStatePtr state,
                               std::string const &dirEndingInSlash);

      protected:
        LuaPath();
        void _init(std::string const &arg0, std::string const &vrjlua_base);

        static std::string
        _findFilePath(std::vector<std::string> const &startingPlaces,
                      std::string const &qualified);
        static std::string _findFilePath(std::string const &startingAt,
                                         std::string const &fn);

        std::string _findJuggler();
        bool _setJugglerEnvironment() const;

        std::string _initialPath;
        std::string _exeDir;
        std::string _root;
        std::string _luaDir;
        std::string _shareDir;
        std::string _appRoot;

        bool _foundJuggler;
        std::string _jugglerRoot;

        bool _valid;
    };

    // -- inline implementations -- /

} // end of vrjLua namespace

#endif // INCLUDED_vrjugglua_LuaPath_h
