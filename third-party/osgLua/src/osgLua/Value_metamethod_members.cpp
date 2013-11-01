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

#include "InternalConfig.h"
#include <osgLua/Value>
#include "ArrayPropertyProxy.h"
#include "Value_metamethods.h"
#include "Value_rawget.inl"
#include "lua_functions.h"

#include <osgLua/introspection/Value>
#include <osgLua/introspection/Type>
#include <osgLua/introspection/variant_cast>
#include <osgLua/introspection/Reflection>
#include <osgLua/introspection/MethodInfo>
#include <osgLua/introspection/PropertyInfo>
#include <osgLua/introspection/Utility>
#include <osgLua/introspection/Exceptions>

#include <osg/NodeVisitor>

#include "LuaIncludeFull.h"

#include <iostream> // for basic_ostream, operator<<, etc
#include <string>   // for string, operator<<, etc
#include <typeinfo> // for type_info
#include <vector>   // for vector, vector<>::iterator

namespace osgLua {

    int Value::_gc(lua_State *L) {
        Value *v = _rawGet(L, 1);
        delete v;
        return 0;
    }

    static const introspection::PropertyInfo *
    lookupProperty(const introspection::Type &type,
                   std::string const &memName) {
        introspection::PropertyInfoList props;
        type.getAllProperties(props);
        if (props.size() > 0) {
            for (unsigned int i = 0; i < props.size(); ++i) {
                if (props[i]->getName() == memName) {
                    return props[i];
                }
            }
        }
        return NULL;
    }

    int Value::_index(lua_State *L) {
        Value *v = _rawGet(L, 1);
        if (lua_isstring(L, 2)) {
            const introspection::Type &type = v->getType();
            if (!type.isDefined()) {
                return luaL_error(L, "Type not defined '%s'",
                                  type.getStdTypeInfo().name());
            }

            const introspection::PropertyInfo *prop =
                lookupProperty(type, lua_tostring(L, 2));
            if (prop) {
                if (prop->isArray()) {
                    ArrayPropertyProxy::pushNew(L, v->get(), prop);
                    return 1;
                } else if (!prop->canGet()) {
                    return luaL_error(L,
                                      "Property '%s' defined as not gettable",
                                      prop->getName().c_str());
                } else {
                    introspection::Value propVal = prop->getValue(v->get());
                    Value::push(L, propVal);
                    return 1;
                }
            }

            // OK, it's not a property, assume it's a method.
            lua_pushvalue(L, 2); // copy the name
            lua_pushcclosure(L, Value::_methodCall, 1);
            return 1;
        }
        // maybe ... if is an integer... access indexed data
        return 0;
    }

    int Value::_methodCall(lua_State *L) {
        int top = lua_gettop(L);

        Value *value = Value::getRequired(L, 1);

        introspection::ValueList vl;
        for (int i = 2; i <= top; ++i) {
            vl.push_back(getValue(L, i));
        }

        try {
            std::string name(lua_tostring(L, lua_upvalueindex(1)));
            const introspection::MethodInfo *method = 0;
            const introspection::Type &type = value->getType();
            method = type.getCompatibleMethod(name, vl, true);

            if (!method && vl.size() > 0) {
                if (vl.back().getType().isNonConstPointer() &&
                    vl.back().getInstanceType().isSubclassOf(
                        introspection::Reflection::getType(
                            "osg::NodeVisitor"))) {
                    /// @todo make this more general instead of a
                    /// visitor-specific hack
                    // OK, we have a pointer to a visitor, let's search again
                    // after dereferencing
                    introspection::Value vPointer = vl.back();
                    vl.pop_back();
                    // Dereference the pointer
                    vl.push_back(
                        *introspection::variant_cast<osg::NodeVisitor *>(
                             vPointer));

                    // Search again for the method
                    method = type.getCompatibleMethod(name, vl, true);
                }
            }
            if (!method) {
                // Couldn't find a method
                int top = lua_gettop(L);
                lua_pushfstring(L, "Error method %s::%s(",
                                type.getName().c_str(),
                                lua_tostring(L, lua_upvalueindex(1)));
                for (introspection::ValueList::iterator i = vl.begin();
                     i != vl.end(); ++i) {
                    lua_pushstring(L, i->getType().getName().c_str());
                    lua_pushstring(L, ",");
                }
                if (!vl.empty()) {
                    lua_pop(L, 1);
                }
                lua_pushstring(L, ") not found");
                lua_concat(L, lua_gettop(L) - top);
                return lua_error(L);
            }

            // OK, we got a method!
            introspection::Value returnedval = method->invoke(value->get(), vl);
            Value::push(L, returnedval);
            return 1;
        }
        catch (introspection::Exception &e) {
            return luaL_error(L, "[%s:%d] %s", __FILE__, __LINE__,
                              e.what().c_str());
        }
        return 0;
    }

    int Value::_newindex(lua_State *L) {

        Value *v = _rawGet(L, 1);

        Value *newVal = get(L, 3);
        if (lua_isstring(L, 2)) {
            const introspection::Type &type = v->getType();
            if (!type.isDefined()) {
                return luaL_error(L, "Type not defined %s",
                                  type.getStdTypeInfo().name());
            }

            const introspection::PropertyInfo *prop =
                lookupProperty(type, lua_tostring(L, 2));
            if (prop) {
                if (!prop->isSimple()) {
                    /// @todo implement other types of properties?
                    return luaL_error(
                        L,
                        "Non-simple properties are not yet settable in osgLua");
                } else if (!prop->canSet()) {
                    return luaL_error(L, "Property %s defined as not settable",
                                      prop->getName().c_str());
                } else {
                    try {
                        prop->setValue(v->get(), newVal->get());
                        return 0;
                    }
                    catch (introspection::Exception &e) {
                        return luaL_error(L, "[%s:%d] %s", __FILE__, __LINE__,
                                          e.what().c_str());
                    }
                }
            }

            return luaL_error(L, "No property '%s' defined in '%s'",
                              lua_tostring(L, 2),
                              type.getQualifiedName().c_str());
        }
        /// @todo error here?
        // maybe ... if is an integer... access indexed data
        return 0;
    }

} // end of osgLua namespace
