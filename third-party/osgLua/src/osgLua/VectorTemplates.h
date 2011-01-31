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

#ifndef OSGLUA_VECTORTEMPLATES
#define OSGLUA_VECTORTEMPLATES

#include "Value_metamethods.h"
#include "BindValue.h"

#include <osgIntrospection/Value>
#include <osgIntrospection/Type>
#include <osg/Referenced>
#include <osg/ref_ptr>

#include <osg/Vec4>
#include <osg/Vec3>
#include <osg/Vec4f>
#include <osg/Vec3f>
#include <osg/Vec4d>
#include <osg/Vec3d>
#include <osg/Matrix>
#include <osg/Matrixf>
#include <osg/Matrixd>

namespace osgLua {


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
	
	} // end of namespace detail
	
	namespace Vector {
	
		template<class T>
		int add(lua_State *L) {
			Value *a = Value::getRequired(L,1);
			Value *b = Value::getRequired(L,2);

			const osgIntrospection::Type &typeA = a->getType();
			const osgIntrospection::Type &typeB = b->getType();

			static const osgIntrospection::Type& myType = 
		  		osgIntrospection::Reflection::getType(extended_typeid<T>());
			if (typeA == myType && typeB == myType) {
		  		osgIntrospection::Value ret = detail::addVectors<T>(a->get(), b->get());
		  		Value::push(L, ret);
		  		return 1;
		  	} else {
				luaL_error(L,"[%s:%d] Could not add instance of %s, %s",__FILE__,__LINE__, typeA.getQualifiedName().c_str(), typeB.getQualifiedName().c_str());
			}
			return 0;
		}
		
		template<class T>
		int sub(lua_State *L) {
			Value *a = Value::getRequired(L,1);
			Value *b = Value::getRequired(L,2);

			const osgIntrospection::Type &typeA = a->getType();
			const osgIntrospection::Type &typeB = b->getType();

			static const osgIntrospection::Type& myType = 
		  		osgIntrospection::Reflection::getType(extended_typeid<T>());
			if (typeA == myType && typeB == myType) {
		  		osgIntrospection::Value ret = detail::subtractVectors<T>(a->get(), b->get());
		  		Value::push(L, ret);
		  		return 1;
		  	} else {
				luaL_error(L,"[%s:%d] Could not subtract instance of %s, %s",__FILE__,__LINE__, typeA.getQualifiedName().c_str(), typeB.getQualifiedName().c_str());
			}
			return 0;
		}
		
		template<class T>
		int unm(lua_State *L) {
			Value *a = Value::getRequired(L,1);
	  		osgIntrospection::Value ret = detail::scaleVector<T>(a->get(), -1);
	  		Value::push(L, ret);
	  		return 1;
		}
		
		template<class T>
		int scale(lua_State *L) {

			double scalar;
			Value * vector = Value::get(L,1);
			if (vector == 0) {
				if (!lua_isnumber(L, 1)) {
					luaL_error(L, "%s:%d Expected a number but get %s",
						__FILE__,__LINE__, lua_typename(L,lua_type(L, 1)) );
				}
				scalar = lua_tonumber(L, 1);
				vector = Value::getRequired(L,2);
			} else {
				if (!lua_isnumber(L, 2)) {
					luaL_error(L, "%s:%d Expected a number but get %s",
						__FILE__,__LINE__, lua_typename(L,lua_type(L, 2)) );
				}
				scalar = lua_tonumber(L, 2);
			}
			
			osgIntrospection::Value ret = detail::scaleVector<T>(vector->get(), scalar);
	  		Value::push(L, ret);
	  		return 1;
	  	}
	  	
	  	template<class T>
	  	bool bind_metamethods(lua_State *L, const osgIntrospection::Type &valT) {
	  		static const osgIntrospection::Type& myType = 
		  		osgIntrospection::Reflection::getType(extended_typeid<T>()); 
		  	if (valT == myType) { 
		  		detail::setVector(L);
		  		lua_pushcfunction(L, &Vector::add<T>); 
		  		lua_setfield(L, -2, "__add"); 
		  		lua_pushcfunction(L, &Vector::sub<T>); 
		  		lua_setfield(L, -2, "__sub"); 
		  		lua_pushcfunction(L, &Vector::unm<T>); 
		  		lua_setfield(L, -2, "__unm"); 
		  		lua_pushcfunction(L, &Vector::scale<T>);
		  		lua_setfield(L, -2, "__mul"); 
		  		lua_pushcfunction(L, &value_metamethods::eq); 
		  		lua_setfield(L, -2, "__eq"); 
		  		lua_pushcfunction(L, &value_metamethods::lt); 
		  		lua_setfield(L, -2, "__lt"); 
		  		lua_pushcfunction(L, &value_metamethods::le); 
		  		lua_setfield(L, -2, "__le"); 
		  		return true;
		  	}
		  	return false;
	  	}
	} // end of Vector namespace
	

} // end of osgLua namespace

#endif
