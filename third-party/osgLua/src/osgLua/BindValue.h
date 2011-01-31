/*
	osgLua: use Lua to access dynamically to osg using osgIntrospection
	Copyright(C) 2006 Jose L. Hidalgo Valiño (PpluX) (pplux at pplux.com)

    This library is open source and may be redistributed and/or modified under  
    the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
    (at your option) any later version.  The full license is in LICENSE file
    included with this distribution, and on the openscenegraph.org website.
    
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
    OpenSceneGraph Public License for more details.
*/

#ifndef OSGLUA_BINDVALUE
#define OSGLUA_BINDVALUE

#include <vrjugglua/LuaIncludeFull.h>

#include <osgIntrospection/Type>

namespace osgLua {
	namespace detail {
        void setVector(lua_State * L);
        
        void setMatrix(lua_State * L);
        
        bool isMatrix(lua_State * L, int index);
        bool isVector(lua_State * L, int index);
        
	} // end of detail namespace

} // end of osgLua namespace

#endif
