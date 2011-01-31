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

#include <osgLua/Value>
#include "Value_metamethods.h"
#include "Type.h"
#include "lua_functions.h"

#include <osgIntrospection/Reflection>
#include <osgIntrospection/MethodInfo>
#include <osgIntrospection/ConstructorInfo>
#include <osgIntrospection/PropertyInfo>
#include <osgIntrospection/Utility>
#include <osgIntrospection/ExtendedTypeInfo>
#include <osgIntrospection/variant_cast>

#include <osg/NodeVisitor>

#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/Matrix>

namespace osgLua {
	static const char OSGLUAVALUEMETATABLE[] = "__isOsgLuaValue";
	
	
	static const char OSGLUAVALUEISMATRIX[] = "__isOsgLuaMatrix";
	static const char OSGLUAVALUEISVECTOR[] = "__isOsgLuaVector";

	static int osgLuaTypeTag(lua_State *L) {
		lua_pushboolean(L, true);
		return 1;
	}
	
	
			
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
    }
	
	bool Value::_hasOsgLuaValueMetatable(lua_State *L, int index) {
		int top = lua_gettop(L);
		index = (index>0)? index : top + index + 1;

		if (lua_isuserdata(L, index))
		{
			lua_getmetatable(L, index);
			lua_pushstring(L, OSGLUAVALUEMETATABLE);
			lua_gettable(L, -2);
			if (lua_tocfunction(L,-1) == &osgLuaTypeTag)
			{
				lua_settop(L,top);
				return true;
			}
		}
		lua_settop(L,top);
		return false;
	}
	
	bool Value::_isMatrix(lua_State *L, int index) {
		int top = lua_gettop(L);
		index = (index>0)? index : top + index + 1;

		if (_hasOsgLuaValueMetatable(L, index))
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
	
	bool Value::_isVector(lua_State *L, int index) {
		int top = lua_gettop(L);
		index = (index>0)? index : top + index + 1;

		if (_hasOsgLuaValueMetatable(L, index))
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

	void Value::_getOrCreateMetatable(lua_State *L, osgIntrospection::Type const& t) {
		// create/get the metatable
		if(luaL_newmetatable(L, t.getQualifiedName().c_str()))
		{
			std::cout << "First time pushing " << t.getQualifiedName() << " to Lua - creating metatable!" << std::endl;

			// tag this as an osgLua value
			lua_pushcfunction(L, osgLuaTypeTag);
			lua_setfield(L, -2, OSGLUAVALUEMETATABLE);
		
			lua_pushcfunction(L, Value::__gc);
			lua_setfield(L, -2, "__gc");	
		
			lua_pushcfunction(L, Value::__index);
			lua_setfield(L, -2, "__index");
		
			lua_pushcfunction(L, Value::__newindex);
			lua_setfield(L, -2, "__newindex");
		
			if (t.getReaderWriter()) {
				/// If we know how to turn it into a string
				lua_pushcfunction(L, value_metamethods::tostring);
			} else {
				lua_pushcfunction(L, value_metamethods::minimal_tostring);
			}
		
			lua_setfield(L, -2, "__tostring");
		
			/// Bind mathematically-inclined values specially
		  	bool success = false;
			#define BIND_VECTOR(TYPE) \
			if (!success) { \
		  		success = Vector::bind_metamethods<TYPE>(L, t); \
		  	}
		  	
		  	BIND_VECTOR(osg::Vec4d)
		  	BIND_VECTOR(osg::Vec4f)
		  	BIND_VECTOR(osg::Vec4ub)
		  	BIND_VECTOR(osg::Vec4s)
		  	BIND_VECTOR(osg::Vec4)
		  	BIND_VECTOR(osg::Vec3d)
		  	BIND_VECTOR(osg::Vec3f)
		  	BIND_VECTOR(osg::Vec3s)
		  	BIND_VECTOR(osg::Vec3b)
		  	BIND_VECTOR(osg::Vec3)
		  	BIND_VECTOR(osg::Vec2d)
		  	BIND_VECTOR(osg::Vec2f)
		  	BIND_VECTOR(osg::Vec2b)
		  	BIND_VECTOR(osg::Vec2s)
		  	BIND_VECTOR(osg::Vec2)
		  	
		  	#undef BIND_VECTOR


			#define BIND_MATRIX(TYPE) \
			if (!success) { \
		  		success = Matrix::bind_metamethods<TYPE>(L, t); \
		  	}
		  	
		  	BIND_MATRIX(osg::Matrixd)
		  	BIND_MATRIX(osg::Matrixf)
		  	BIND_MATRIX(osg::Matrix)
		  	
		  	#undef BIND_MATRIX
		  		
		}
	}


} // end of osgLua namespace
