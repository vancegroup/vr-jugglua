/** @file
        @brief Header to close extern "C" {} if needed for the Lua interface.

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

/// No header guards - multiple inclusion acceptable when included in pairs.

// Internal Includes
// - none

// Library/third-party includes
// - none

// Standard includes
// - none

#ifndef VRJLUA_LUACINTERFACE_ACTIVE
#error "Must include LuaCInterface Begin and End in pairs!"
#endif

#undef VRJLUA_LUACINTERFACE_ACTIVE

#if defined(__cplusplus) && !defined(BUILD_LUA_AS_CPP)
}
#endif
