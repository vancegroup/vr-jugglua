/** @file
        @brief Header

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

#pragma once
#ifndef INCLUDED_LuaPathUpdater_h_GUID_ebe0b3e8_34e2_442c_afa8_2ffbdf2f4864
#define INCLUDED_LuaPathUpdater_h_GUID_ebe0b3e8_34e2_442c_afa8_2ffbdf2f4864

// Internal Includes
#include <vrjugglua/AddToLuaPaths.h>

// Library/third-party includes
#include <luabind/object.hpp>

// Standard includes
// - none

namespace vrjLua {

    template <typename Contained> class SearchPathUpdater {
      public:
        SearchPathUpdater(lua_State *L)
            : package(luabind::globals(L)["package"])
            , _p(luabind::object_cast<std::string>(
                  package[Contained::getTableKey()])) {}
        ~SearchPathUpdater() {
            package[Contained::getTableKey()] = _p.toString();
        }
        template <typename DirectoryTag>
        void extend(detail::DirectoryBase<DirectoryTag> const &d) {
            _p.extend(d);
        }

      private:
        luabind::object package;
        Contained _p;
    };

    typedef SearchPathUpdater<LuaSearchPath> LuaSearchPathUpdater;
    typedef SearchPathUpdater<LuaCSearchPath> LuaCSearchPathUpdater;

} // end of namespace vrjLua

#endif // INCLUDED_LuaPathUpdater_h_GUID_ebe0b3e8_34e2_442c_afa8_2ffbdf2f4864
