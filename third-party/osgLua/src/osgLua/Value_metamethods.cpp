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
#include <osgLua/Value>
#include "Value_rawget.inl"
#include "Type.h"
#include "lua_functions.h"

#include <osgIntrospection/Reflection>
#include <osgIntrospection/MethodInfo>
#include <osgIntrospection/ConstructorInfo>
#include <osgIntrospection/Utility>
#include <osgIntrospection/ExtendedTypeInfo>
#include <osgIntrospection/PropertyInfo>
#include <osgIntrospection/variant_cast>

#include <osg/Vec3>
#include <osg/Vec3d>
#include <osg/Vec3f>
#include <osg/Vec4>
#include <osg/Vec4d>
#include <osg/Vec4f>

#include <sstream>

namespace osgLua {

	namespace value_metamethods {
		
		int tostring(lua_State *L) {
			Value *a = Value::getRequired(L,1);

			lua_pushstring(L, a->get().toString().c_str());
			return 1;
		}
	
		int minimal_tostring(lua_State *L) {
			Value *a = Value::getRequired(L,1);
			std::stringstream s;
			s << "osgLua::Value (" << a->get().getType().getQualifiedName() << ") " << a;
			lua_pushstring(L, s.str().c_str());
			return 1;
		}

		int eq(lua_State *L) {
			Value *a = Value::getRequired(L,1);
			Value *b = Value::getRequired(L,2);

			bool ret;
			try {
				ret = a->get() == b->get();
			} catch(osgIntrospection::Exception &e) {
				luaL_error(L,"[%s:%d] %s",__FILE__,__LINE__,e.what().c_str());
			}

			lua_pushboolean(L, ret);

			return 1;
		}

		int lt(lua_State *L) {
			Value *a = Value::getRequired(L,1);

			Value *b = Value::getRequired(L,2);
			bool ret;
			try {
				ret = a->get() < b->get();
			} catch(osgIntrospection::Exception &e) {
				luaL_error(L,"[%s:%d] %s",__FILE__,__LINE__,e.what().c_str());
			}

			lua_pushboolean(L, ret);

			return 1;
		}

		int le(lua_State *L) {
			Value *a = Value::getRequired(L,1);

			Value *b = Value::getRequired(L,2);
			bool ret;
			try {
				ret = a->get() <= b->get();
			} catch(osgIntrospection::Exception &e) {
				luaL_error(L,"[%s:%d] %s",__FILE__,__LINE__,e.what().c_str());
			}

			lua_pushboolean(L, ret);

			return 1;
		}
		
		
	
		
	
	} // end of value_metamethods namespace
	
	int Value::__newindex(lua_State *L) {
		
		Value *v = _rawGet(L,1);
	
		Value *newVal = get(L,3);
		if (lua_isstring(L,2))
		{
			const osgIntrospection::Type &type = v->getType();
			if (!type.isDefined())
			{
				luaL_error(L, "Type not defined %s", 
					type.getStdTypeInfo().name());
			}
			//std::string cname = type.getQualifiedName();
		
			const char * memberName = lua_tostring(L, 2);
			std::string memName(memberName);
			osgIntrospection::PropertyInfoList props;
			type.getAllProperties(props);
			if (props.size() > 0) {
				for (unsigned int i = 0; i < props.size(); ++i) {
					if (props[i]->getName() == memName) {
						if (props[i]->isIndexed()) {
							/// @todo implement indexed properties
							luaL_error(L, "Indexed properties are not yet implemented in osgLua");
						} else if (!props[i]->canSet()) {
							luaL_error(L, "Property %s defined as not settable", props[i]->getName().c_str());
						} else {
							std::cout << "Setting a property named " << props[i]->getName() << std::endl;
							props[i]->setValue(v->get(), newVal->get());
							return 0;
						}
					}
				}
			}
		
			luaL_error(L, "No property %s defined in %s", memberName, type.getQualifiedName().c_str());
	
		}
		// maybe ... if is an integer... access indexed data
		return 0;	
	}

} // end of osgLua namespace
