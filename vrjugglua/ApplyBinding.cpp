/**
        @file
        @brief Implementation

        @date 2013

        @author
        Ryan Pavlik
        <rpavlik@iastate.edu> and <abiryan@ryand.net>
        http://academic.cleardefinition.com/
        Iowa State University Virtual Reality Applications Center
        Human-Computer Interaction Graduate Program
*/

//          Copyright Iowa State University 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// Internal Includes
#include "ApplyBinding.h"

#include "VRJLua_C_Interface.h"
#include "OsgAppProxy.h"
#include "LuaPath.h"
#include "LuaPathUpdater.h"

#include "osgLuaBind.h"
#include "binding_detail/BindOsgToLua.h"

#include "binding_detail/BindKernelToLua.h"
#include "binding_detail/BindSonixToLua.h"
#include "binding_detail/BindGadgetInterfacesToLua.h"
#include "binding_detail/BindRunBufferToLua.h"
#include "binding_detail/BindvrjLuaToLua.h"

// Library/third-party includes
#include <luabind/luabind.hpp>
#include <luabind/class_info.hpp>

// Standard includes
#include <iostream>
#include <sstream>
#include <string> // for operator<<, string

namespace {
    // From the Luabind docs
    int add_file_and_line(lua_State *L) {
        lua_Debug d;
        lua_getstack(L, 1, &d);
        lua_getinfo(L, "Sln", &d);
        std::string err = lua_tostring(L, -1);
        lua_pop(L, 1);
        std::stringstream msg;
        msg << d.short_src << ":" << d.currentline;

        if (d.name != 0) {
            msg << "(" << d.namewhat << " " << d.name << ")";
        }
        msg << " " << err;
        lua_pushstring(L, msg.str().c_str());
        std::cerr << std::endl << msg.str() << std::endl;
        return 1;
    }
} // end of namespace

namespace vrjLua {
    void applyBinding(lua_State *L) {

        // Connect LuaBind to this state
        try {
            luabind::open(L);
            luabind::bind_class_info(L);
        }
        catch (const std::exception &e) {
            std::cerr << "Caught exception connecting luabind and class_info: "
                      << e.what() << std::endl;
            throw;
        }

        /// Apply our bindings to this state

        // Extend the lua script search path for "require"
        LuaPath &lp = LuaPath::instance();
        {
            LuaSearchPathUpdater searchpath(L);
            searchpath.extend(SearchDirectory(lp.getLuaDir()));
            searchpath.extend(RootDirectory(lp.getRootDir()));
        }

        {
            /// @todo c search path
            // LuaCSearchPathUpdater csearchpath(L);
        }

        // osgLua
        bindOsgToLua(L);

        // vrjugglua
        bindKernelToLua(L);
        bindSonixToLua(L);
        bindGadgetInterfacesToLua(L);
        bindRunBufferToLua(L);
        BindvrjLuaToLua(L);

        OsgAppProxy::bindToLua(L);

        // Set up traceback...
        luabind::set_pcall_callback(&add_file_and_line);

        // Load the vrjlua init script
        luabind::globals(L)["require"]("vrjlua-init");

        // set up global for debug mode
        /// @todo make this work
        /*
                        luabind::module(L.get(), "vrjlua")[
                                                                luabind::def_readwrite(boost::ref(LuaScript::exitOnError))
                                                                                                   ];

        */
    }
}
