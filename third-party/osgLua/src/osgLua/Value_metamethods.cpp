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

#include "Value_metamethods.h"
#include "Value.h"
#include "Type.h"
#include "lua_functions.h"

#include <osgIntrospection/Reflection>
#include <osgIntrospection/MethodInfo>
#include <osgIntrospection/ConstructorInfo>
#include <osgIntrospection/Utility>
#include <osgIntrospection/ExtendedTypeInfo>
#include <osgIntrospection/variant_cast>

#include <osg/Vec3>
#include <osg/Vec3d>
#include <osg/Vec3f>
#include <osg/Vec4>
#include <osg/Vec4d>
#include <osg/Vec4f>

namespace osgLua {
	
	int metamethods::add(lua_State *L) {
		Value *a = Value::get(L,1);
		if (a == 0) {
			luaL_error(L, "%s:%d Expected a osgLua userdata but get %s",
				__FILE__,__LINE__, lua_typename(L,lua_type(L, 1)) ) ;
		}
		
		Value *b = Value::get(L,2);
		if (b == 0) {
			luaL_error(L, "%s:%d Expected a osgLua userdata but get %s",
				__FILE__,__LINE__, lua_typename(L,lua_type(L, 2)) ) ;
		}
		
		
		
		const osgIntrospection::Type &typeA = a->getType();
		const osgIntrospection::Type &typeB = b->getType();
		std::cout << "Finished grabbing params: " << typeA.getQualifiedName() << ", " << typeB.getQualifiedName() << std::endl;

		bool success = false;
		#define VECTOR_MATH(TYPE, NAME) \
		std::cout << "Before " << #TYPE << std::endl; \
		static const osgIntrospection::Type& NAME = \
	  		osgIntrospection::Reflection::getType(extended_typeid<TYPE>()); \
	  	if (!success && typeA == NAME && typeB == NAME) { \
	  		std::cout << "Match!" << std::endl; \
	  		success = true; \
	  		osgIntrospection::Value ret = detail::addVectors<TYPE>(a->get(), b->get()); \
	  		std::cout << "Result is of type " << ret.getType().getQualifiedName() << ", pushing... " << std::endl; \
	  		Value::push(L, ret); \
	  	}
	  	
	  	VECTOR_MATH(osg::Vec4d, tvec4d)
	  	VECTOR_MATH(osg::Vec4, tvec4)
	  	VECTOR_MATH(osg::Vec4f, tvec4f)

	  	VECTOR_MATH(osg::Vec3d, tvec3d)
	  	VECTOR_MATH(osg::Vec3, tvec3)
	  	VECTOR_MATH(osg::Vec3f, tvec3f)
	  	
	  	#undef VECTOR_MATH
	  	
	  	if (success) {
			return 1;
		} else {
			luaL_error(L,"[%s:%d] Could not add instance of %s, %s",__FILE__,__LINE__, typeA.getQualifiedName().c_str(), typeB.getQualifiedName().c_str());
		}
		return 0;
		
	}
	
	int metamethods::sub(lua_State *L) {
		Value *a = Value::get(L,1);
		if (a == 0) {
			luaL_error(L, "%s:%d Expected a osgLua userdata but get %s",
				__FILE__,__LINE__, lua_typename(L,lua_type(L, 1)) ) ;
		}
		
		Value *b = Value::get(L,2);
		if (b == 0) {
			luaL_error(L, "%s:%d Expected a osgLua userdata but get %s",
				__FILE__,__LINE__, lua_typename(L,lua_type(L, 2)) ) ;
		}
		
		
		
		const osgIntrospection::Type &typeA = a->getType();
		const osgIntrospection::Type &typeB = b->getType();
		std::cout << "Finished grabbing params: " << typeA.getQualifiedName() << ", " << typeB.getQualifiedName() << std::endl;

		bool success = false;
		#define VECTOR_MATH(TYPE, NAME) \
		std::cout << "Before " << #TYPE << std::endl; \
		static const osgIntrospection::Type& NAME = \
	  		osgIntrospection::Reflection::getType(extended_typeid<TYPE>()); \
	  	if (!success && typeA == NAME && typeB == NAME) { \
	  		std::cout << "Match!" << std::endl; \
	  		success = true; \
	  		osgIntrospection::Value ret = detail::subtractVectors<TYPE>(a->get(), b->get()); \
	  		std::cout << "Result is of type " << ret.getType().getQualifiedName() << ", pushing... " << std::endl; \
	  		Value::push(L, ret); \
	  	}
	  	
	  	VECTOR_MATH(osg::Vec4d, tvec4d)
	  	VECTOR_MATH(osg::Vec4, tvec4)
	  	VECTOR_MATH(osg::Vec4f, tvec4f)

	  	VECTOR_MATH(osg::Vec3d, tvec3d)
	  	VECTOR_MATH(osg::Vec3, tvec3)
	  	VECTOR_MATH(osg::Vec3f, tvec3f)
	  	
	  	#undef VECTOR_MATH
	  	
	  	if (success) {
			return 1;
		} else {
			luaL_error(L,"[%s:%d] Could not subtract instance of %s, %s",__FILE__,__LINE__, typeA.getQualifiedName().c_str(), typeB.getQualifiedName().c_str());
		}
		return 0;
	}
	
	int metamethods::mul(lua_State *L) {
		/// @todo
		return 0;
	}
	
	int metamethods::tostring(lua_State *L) {
		Value *a = Value::get(L,1);
		if (a == 0) {
			luaL_error(L, "%s:%d Expected a osgLua userdata but get %s",
				__FILE__,__LINE__, lua_typename(L,lua_type(L, 1)) ) ;
		}
		
		lua_pushstring(L, a->get().toString().c_str());
		return 1;
	}

} // end of osgLua namespace
