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

#include <osgLua/Value>
#include "Value_metamethods.h"
#include "Value_rawget.inl"
#include "Type.h"
#include "lua_functions.h"

#include "LuaIncludeFull.h"

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

	int Value::_gc(lua_State *L) {
		Value *v = _rawGet(L,1);
		delete v;
		return 0;
	}

	int Value::_index(lua_State *L) {
		Value *v = _rawGet(L,1);
		if (lua_isstring(L,2)) {
			const osgIntrospection::Type &type = v->getType();
			if (!type.isDefined()) {
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
						} else if (!props[i]->canGet()) {
							luaL_error(L, "Property %s defined as not gettable", props[i]->getName().c_str());
						} else {
							//std::cout << "Getting a property named " << props[i]->getName() << std::endl;
							osgIntrospection::Value propVal = props[i]->getValue(v->get());
							Value::push(L, propVal);
							return 1;
						}
					}
				}
			}

			lua_pushvalue(L,2); // copy the name
			lua_pushcclosure(L, Value::_methodCall,1);
			return 1;
		}
		// maybe ... if is an integer... access indexed data
		return 0;
	}


	int Value::_methodCall(lua_State *L) {
		int top = lua_gettop(L);

		Value *value = Value::getRequired(L,1);

		osgIntrospection::ValueList vl;
		for (int i = 2; i <= top; ++i) {
			vl.push_back(getValue(L, i));
		}

		try {
			std::string name(lua_tostring(L, lua_upvalueindex(1)));
			const osgIntrospection::MethodInfo *method = 0;
			const osgIntrospection::Type &type = value->getType();
			method = type.getCompatibleMethod(name,vl, true);
			/* This code is no longer needed if getCompatibleMethod
			 * finds methods in base types correctly.
			if (!method)
			{
				//manual method finding... d'oh!
				for(int i = 0; i < type.getNumBaseTypes() && !method; ++i)
				{
					const osgIntrospection::Type &base =
						type.getBaseType(i);
					if (!base.isDefined()) continue;
					method = base.getCompatibleMethod(name,vl,false);
				}
			}
			*/

			if (!method && vl.size() > 0) {
				if (vl.back().getType().isNonConstPointer() &&
				        vl.back().getInstanceType().isSubclassOf(osgIntrospection::Reflection::getType("osg::NodeVisitor"))) {
					// OK, we have a pointer to a visitor, let's search again after dereferencing
					osgIntrospection::Value vPointer = vl.back();
					vl.pop_back();
					// Dereference the pointer
					vl.push_back(*osgIntrospection::variant_cast<osg::NodeVisitor*>(vPointer));

					// Search again for the method
					method = type.getCompatibleMethod(name,vl, true);
				}
			}
			if (!method) {
				// Couldn't find a method
				int top = lua_gettop(L);
				lua_pushfstring(L, "Error method %s::%s(",
				                type.getName().c_str(),
				                lua_tostring(L,lua_upvalueindex(1))
				               );
				for (osgIntrospection::ValueList::iterator
				        i = vl.begin(); i != vl.end(); ++i) {
					lua_pushstring(L, i->getType().getName().c_str());
					lua_pushstring(L,",");
				}
				if (!vl.empty()) lua_pop(L,1);
				lua_pushstring(L,") not found");
				lua_concat(L,lua_gettop(L) - top);
				lua_error(L);

			}

			// OK, we got a method!
			osgIntrospection::Value returnedval = method->invoke(value->get(), vl);
			Value::push(L, returnedval);
			return 1;
		} catch (osgIntrospection::Exception &e) {
			luaL_error(L,"[%s:%d] %s",__FILE__,__LINE__,e.what().c_str());
		}
		return 0;

	}

	int Value::_newindex(lua_State *L) {

		Value *v = _rawGet(L,1);

		Value *newVal = get(L,3);
		if (lua_isstring(L,2)) {
			const osgIntrospection::Type &type = v->getType();
			if (!type.isDefined()) {
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
