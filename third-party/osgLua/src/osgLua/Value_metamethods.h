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

#ifndef OSGLUA_VALUE_METAMETHODS
#define OSGLUA_VALUE_METAMETHODS


#include <vrjugglua/LuaIncludeFull.h>

#include <osgIntrospection/Value>
#include <osgIntrospection/Type>
#include <osg/Referenced>
#include <osg/ref_ptr>

namespace osgLua {
	
	namespace metamethods {
		/// @todo make these templates so we don't have to re-figure out the type
		int add(lua_State *L);
		int sub(lua_State *L);
		int unmVec(lua_State *L);
		int scaleVec(lua_State *L);
		
		int tostring(lua_State *L);
		
		int eq(lua_State *L);
		int lt(lua_State *L);
		int le(lua_State *L);
	}
	
	

	namespace detail {
		template<class T>
		T addVectors(osgIntrospection::Value const& a, osgIntrospection::Value const& b) {
			return osgIntrospection::variant_cast<T>(a) + osgIntrospection::variant_cast<T>(b);
		}
		
		template<class T>
		T subtractVectors(osgIntrospection::Value const& a, osgIntrospection::Value const& b) {
			return osgIntrospection::variant_cast<T>(a) - osgIntrospection::variant_cast<T>(b);
		}
		
		template<class T>
		T scaleVector(osgIntrospection::Value const& vector, double scalar) {
			return osgIntrospection::variant_cast<T>(vector) * scalar;
		}
		
		template<class T>
		T multMatrices(osgIntrospection::Value const& a, osgIntrospection::Value const& b) {
			return osgIntrospection::variant_cast<T>(a) * osgIntrospection::variant_cast<T>(b);
		}
	
	} // end of namespace detail

} // end of osgLua namespace

#endif
