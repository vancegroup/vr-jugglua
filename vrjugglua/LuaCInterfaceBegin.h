/** @file
        @brief Header to open extern "C" {} if needed for the Lua interface.

        Include this file, declare or include what you need to, then include
        LuaCInterfaceEnd.h

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
#include <vrjugglua/VRJLuaConfig.h>

// Library/third-party includes
// - none

// Standard includes
// - none

#ifdef VRJLUA_LUACINTERFACE_ACTIVE
#error "Must include LuaCInterface Begin and End in pairs!"
#endif

#define VRJLUA_LUACINTERFACE_ACTIVE

#if defined(__cplusplus) && !defined(BUILD_LUA_AS_CPP)
extern "C" {
#endif
