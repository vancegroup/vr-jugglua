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

#ifndef OSGLUA_MATRIXTEMPLATES
#define OSGLUA_MATRIXTEMPLATES

#include "MathValueTags.h"

#include <osgIntrospection/Reflection>
#include <osgIntrospection/Value>
#include <osgIntrospection/Type>
#include <osgIntrospection/variant_cast>
#include <osgIntrospection/ExtendedTypeInfo>

#include <osg/Vec4>
#include <osg/Vec3>
#include <osg/Vec4f>
#include <osg/Vec3f>
#include <osg/Vec4d>
#include <osg/Vec3d>

namespace osgLua {


	namespace detail {
		template<class T>
		T multMatrices(osgIntrospection::Value const& a, osgIntrospection::Value const& b) {
			return osgIntrospection::variant_cast<T>(a) * osgIntrospection::variant_cast<T>(b);
		}
		
		template<class Mat, class Vec>
		Vec xformVec(osgIntrospection::Value const& theVec, osgIntrospection::Value const& theMat) {
			return osgIntrospection::variant_cast<Vec>(theVec) * osgIntrospection::variant_cast<Mat>(theMat);
		}
	
	} // end of namespace detail

	
	namespace Matrix {
		template<class Mat, class Vec>
		bool xform(lua_State *L, Value *a, Value *b) {
			const osgIntrospection::Type &typeA = a->getType();
			const osgIntrospection::Type &typeB = b->getType();
			static const osgIntrospection::Type& vecType = 
		  		osgIntrospection::Reflection::getType(extended_typeid<Vec>());
			static const osgIntrospection::Type& matType = 
		  		osgIntrospection::Reflection::getType(extended_typeid<Mat>());
			if (typeA == vecType && typeB == matType) {
		  		osgIntrospection::Value ret = detail::xformVec<Mat, Vec>(a->get(), b->get());
		  		Value::push(L, ret);
		  		return true;
		  	}
		  	return false;
		}
		
		template<class T>
		int mul(lua_State *L) {
			Value *a = Value::getRequired(L,1);
			Value *b = Value::getRequired(L,2);

			const osgIntrospection::Type &typeA = a->getType();
			const osgIntrospection::Type &typeB = b->getType();

			static const osgIntrospection::Type& myType = 
		  		osgIntrospection::Reflection::getType(extended_typeid<T>());
			if (typeA == myType && typeB == myType) {
				/// matrix multiplication
		  		osgIntrospection::Value ret = detail::multMatrices<T>(a->get(), b->get());
		  		Value::push(L, ret);
		  		return 1;
		  	}
	  		if (typeB == myType) {
	  			/// transform vector by matrix
	  			if (xform<T, osg::Vec3f>(L, a, b)) {
	  				return 1;
	  			}
	  			if (xform<T, osg::Vec3d>(L, a, b)) {
	  				return 1;
	  			}
	  			if (xform<T, osg::Vec3>(L, a, b)) {
	  				return 1;
	  			}
	  			if (xform<T, osg::Vec4f>(L, a, b)) {
	  				return 1;
	  			}
	  			if (xform<T, osg::Vec4d>(L, a, b)) {
	  				return 1;
	  			}
	  			if (xform<T, osg::Vec4>(L, a, b)) {
	  				return 1;
	  			}
	  		}
		  		
		  	luaL_error(L,"[%s:%d] Could not multiply instances of %s, %s",__FILE__,__LINE__, typeA.getQualifiedName().c_str(), typeB.getQualifiedName().c_str());
			return 0;
		}
		
		template<class T>
		int eq(lua_State *L) {
			Value *a = Value::getRequired(L,1);
			Value *b = Value::getRequired(L,2);

			const osgIntrospection::Type &typeA = a->getType();
			const osgIntrospection::Type &typeB = b->getType();

			static const osgIntrospection::Type& myType = 
		  		osgIntrospection::Reflection::getType(extended_typeid<T>());
			if (typeA == myType && typeB == myType) {
				bool ret = ( osgIntrospection::variant_cast<T>(a->get()) == osgIntrospection::variant_cast<T>(b->get()) );  		
				lua_pushboolean(L, ret);
		  		return 1;
		  	} else {
		  		/// @todo figure out why line 11 in osglua-matrixmath.lua fails without this
		  		//return value_metamethods::eq(L);
				luaL_error(L,"[%s:%d] Could not compare instances of %s, %s, in comparator for %s",__FILE__,__LINE__,
					typeA.getQualifiedName().c_str(),
					typeB.getQualifiedName().c_str(),
					myType.getQualifiedName().c_str());
			}
			return 0;
		}
		
		template<class T>
		int lt(lua_State *L) {
			Value *a = Value::getRequired(L,1);
			Value *b = Value::getRequired(L,2);

			const osgIntrospection::Type &typeA = a->getType();
			const osgIntrospection::Type &typeB = b->getType();

			static const osgIntrospection::Type& myType = 
		  		osgIntrospection::Reflection::getType(extended_typeid<T>());
			if (typeA == myType && typeB == myType) {
				bool ret = ( osgIntrospection::variant_cast<T>(a->get()) < osgIntrospection::variant_cast<T>(b->get()) );  		
				lua_pushboolean(L, ret);
		  		return 1;
		  	} else {
				luaL_error(L,"[%s:%d] Could not compare instances of %s, %s, in comparator for %s",__FILE__,__LINE__,
					typeA.getQualifiedName().c_str(),
					typeB.getQualifiedName().c_str(),
					myType.getQualifiedName().c_str());
			}
			return 0;
		}
		
		template<class T>
	  	bool bind_metamethods(lua_State *L, const osgIntrospection::Type &valT) {
	  		static const osgIntrospection::Type& myType = 
		  		osgIntrospection::Reflection::getType(extended_typeid<T>()); 
		  	if (valT == myType) { 
		  		detail::setMatrix(L);
		  		lua_pushcfunction(L, &Matrix::mul<T>); 
		  		lua_setfield(L, -2, "__mul"); 
		  		lua_pushcfunction(L, &Matrix::eq<T>); 
		  		lua_setfield(L, -2, "__eq"); 
		  		lua_pushcfunction(L, &Matrix::lt<T>); 
		  		lua_setfield(L, -2, "__lt");
		  		return true;
		  	}
		  	return false;
	  	}	
	} // end of Matrix namespace
	
	
	

} // end of osgLua namespace

#endif
