/**	@file
        @brief	header exposing binding function for vrjKernel and standard
                        components

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
#ifndef INCLUDED_vrjugglua_binding_detail_BindKernelToLua_h
#define INCLUDED_vrjugglua_binding_detail_BindKernelToLua_h

// Local includes
#include <vrjugglua/LuaStateFwd.h>

// Library/third-party includes
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

// Standard includes
// - none

namespace vrjLua {

    void bindKernelToLua(lua_State *L);

} // end of vrjLua namespace

#endif // INCLUDED_vrjugglua_binding_detail_BindKernelToLua_h
