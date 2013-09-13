/**	@file
        @brief	implementation of binding using osgLua

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

// Local includes
#include "BindOsgToLua.h"

// Library/third-party includes
#include <osgLua/LoadWrapper>
#include <osgLua/Value>

// Standard includes
#include <iostream>

namespace vrjLua {

    void bindOsgToLua(lua_State *L) {
#ifdef VERBOSE
        std::cerr << "Started binding OSG to Lua..." << std::flush << std::endl;
#endif
        osgLua::open(L);
        bool ret = true;

        try {
            ret = osgLua::loadWrapper(L, "OpenThreads") &&
                  osgLua::loadWrapper(L, "osg");
        }
        catch (std::exception &e) {
            std::cerr
                << "ERROR: Caught an exception trying to load osgwrappers : "
                << e.what() << std::endl;
            std::cerr << "Make sure you have the osgwrappers installed in "
                         "their default location with respect to OSG itself."
                      << std::endl;
            std::cerr << "Cannot continue, exiting!" << std::endl;
            std::terminate();
        }

        if (!ret) {
            std::cerr << "Failure return code, trying to load wrappers for osg!"
                      << std::endl;
            lua_pushstring(
                L, "Failure return code, trying to load wrappers for osg!");
            lua_error(L);
            std::terminate();
        }
    }

} // end of vrjLua namespace
