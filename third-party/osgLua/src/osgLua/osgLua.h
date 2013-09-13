/*
        osgLua: use Lua to access dynamically to osg using osgIntrospection
        Copyright(C) 2006 Jose L. Hidalgo Valiño (PpluX) (pplux at pplux.com)
        Copyright(C) 2010-2011 Iowa State University (Author: Ryan Pavlik
   <rpavlik@acm.org> )

    This library is open source and may be redistributed and/or modified under
    the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or
    (at your option) any later version.  The full license is in LICENSE file
    included with this distribution, and on the openscenegraph.org website.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    OpenSceneGraph Public License for more details.
*/

#ifndef OSGLUA_MAIN
#define OSGLUA_MAIN

#include <osgLua/LuaInclude>

OSGLUA_LUA_INTERFACE_BEGIN
int luaopen_osgLua(lua_State *L);
int luaopen_libosgLua(lua_State *L);
OSGLUA_LUA_INTERFACE_END

namespace osgLua {
    void get(lua_State *L);
    int lua_loadWrapper(lua_State *L);
    int lua_loadObjectFile(lua_State *L);
    int lua_saveObjectFile(lua_State *L);
}

#endif
