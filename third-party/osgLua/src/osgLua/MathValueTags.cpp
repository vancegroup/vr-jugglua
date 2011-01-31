/*
	osgLua: use Lua to access dynamically to osg using osgIntrospection
	Copyright(C) 2006 Jose L. Hidalgo Valiño (PpluX) (pplux at pplux.com)
	Copyright(C) 2010-2011 Iowa State University (Author: Ryan Pavlik <rpavlik@acm.org> )

    This library is open source and may be redistributed and/or modified under  
    the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
    (at your option) any later version.  The full license is in LICENSE file
    included with this distribution, and on the openscenegraph.org website.
    
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
    OpenSceneGraph Public License for more details.
*/

#include "MathValueTags.h"
#include <osgLua/Value>

namespace osgLua {
	static const char OSGLUAVALUEISMATRIX[] = "__isOsgLuaMatrix";
	static const char OSGLUAVALUEISVECTOR[] = "__isOsgLuaVector";
			
	static int matrixTag(lua_State *L) {
		lua_pushboolean(L, true);
		return 1;
	}
	
	static int vectorTag(lua_State *L) {
		lua_pushboolean(L, true);
		return 1;
	}
	
	namespace detail {
        void setVector(lua_State * L) {	
			lua_pushcfunction(L, vectorTag);
			lua_setfield(L, -2, OSGLUAVALUEISVECTOR);
        }
        
        void setMatrix(lua_State * L){	
			lua_pushcfunction(L, matrixTag);
			lua_setfield(L, -2, OSGLUAVALUEISMATRIX);
        }
        
        
        bool isMatrix(lua_State * L, int index) {
			int top = lua_gettop(L);
			index = (index>0)? index : top + index + 1;
	
			if (Value::get(L, index))
			{
				lua_getmetatable(L, index);
				lua_pushstring(L, OSGLUAVALUEISMATRIX);
				lua_gettable(L, -2);
				if (lua_tocfunction(L,-1) == &matrixTag)
				{
					lua_settop(L,top);
					return true;
				}
			}
			lua_settop(L,top);
			return false;
		}

        bool isVector(lua_State * L, int index) {
			int top = lua_gettop(L);
			index = (index>0)? index : top + index + 1;
	
			if (Value::get(L, index))
			{
				lua_getmetatable(L, index);
				lua_pushstring(L, OSGLUAVALUEISVECTOR);
				lua_gettable(L, -2);
				if (lua_tocfunction(L,-1) == &vectorTag)
				{
					lua_settop(L,top);
					return true;
				}
			}
			lua_settop(L,top);
			return false;
		}
    }
	

} // end of osgLua namespace
