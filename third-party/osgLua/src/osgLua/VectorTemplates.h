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

#ifndef OSGLUA_VECTORTEMPLATES
#define OSGLUA_VECTORTEMPLATES

#include "MathValueTags.h"
#include "LuaIncludeFull.h"

#include <osgLua/introspection/Reflection>
#include <osgLua/introspection/ExtendedTypeInfo>
#include <osgLua/introspection/Value>
#include <osgLua/introspection/Type>
#include <osgLua/introspection/variant_cast>

namespace osgLua {


	namespace detail {
		template<class T>
		T scaleVector(introspection::Value const& vector, double scalar) {
			return introspection::variant_cast<T>(vector) * scalar;
		}

	} // end of namespace detail

	namespace Vector {

		/// Shared function template to handle both addition and subtraction.
		template<class T, typename FunctorType>
		int apply_binary_typed_predicate(lua_State *L, FunctorType predicate) {
			Value *a = Value::getRequired(L, 1);
			Value *b = Value::getRequired(L, 2);

			const introspection::Type &typeA = a->getType();
			const introspection::Type &typeB = b->getType();

			static const introspection::Type& myType =
			    introspection::Reflection::getType(extended_typeid<T>());
			if (typeA == myType && typeB == myType) {
				introspection::Value ret = predicate(introspection::variant_cast<T>(a->get()), introspection::variant_cast<T>(b->get()));
				Value::push(L, ret);
				return 1;
			} else {
				luaL_error(L, "[%s:%d] Could not perform math on instance of %s, %s", __FILE__, __LINE__, typeA.getQualifiedName().c_str(), typeB.getQualifiedName().c_str());
			}
			return 0;
		}

		template<class T>
		int add(lua_State *L) {
			return apply_binary_typed_predicate<T>(L, std::plus<T>());
		}

		template<class T>
		int sub(lua_State *L) {
			return apply_binary_typed_predicate<T>(L, std::minus<T>());
		}

		template<class T>
		int unm(lua_State *L) {
			Value *a = Value::getRequired(L, 1);
			introspection::Value ret = detail::scaleVector<T>(a->get(), -1);
			Value::push(L, ret);
			return 1;
		}

		template<class T>
		int mul(lua_State *L) {
			if (detail::isVector(L, 1) && detail::isMatrix(L, 2)) {
				// we should be transforming - use the matrix's metatable.
				int top = lua_gettop(L);
				lua_getmetatable(L, 2);
				lua_pushstring(L, "__mul");
				lua_gettable(L, -2);
				lua_CFunction multFunc = lua_tocfunction(L, -1);
				lua_settop(L, top);
				return (*multFunc)(L);
			}

			// we are scaling
			double scalar = 0;
			Value * vector = NULL;
			for (int vecIdx = 1, scalarIdx = 2;
			        vecIdx <= 2;
			        vecIdx++, scalarIdx--) {

				if (detail::isVector(L, vecIdx)) {
					vector = Value::get(L, vecIdx);
					if (!lua_isnumber(L, scalarIdx)) {
						luaL_error(L, "%s:%d Expected a number but get %s",
						           __FILE__, __LINE__, lua_typename(L, lua_type(L, scalarIdx)));
					}
					scalar = lua_tonumber(L, scalarIdx);
					break;
				}
			}

			if (!vector) {
				luaL_error(L, "%s:%d In a vector multiplication metamethod, but didn't find a vector as an argument! (%s, %s)",
				           __FILE__, __LINE__, lua_typename(L, lua_type(L, 1)), lua_typename(L, lua_type(L, 2)));
			}

			introspection::Value ret = detail::scaleVector<T>(vector->get(), scalar);
			Value::push(L, ret);
			return 1;
		}

		template<class T>
		bool bind_metamethods(lua_State *L, const introspection::Type &valT) {
			static const introspection::Type& myType =
			    introspection::Reflection::getType(extended_typeid<T>());
			if (valT == myType) {
				detail::setVector(L);
				lua_pushcfunction(L, &Vector::add<T>);
				lua_setfield(L, -2, "__add");
				lua_pushcfunction(L, &Vector::sub<T>);
				lua_setfield(L, -2, "__sub");
				lua_pushcfunction(L, &Vector::unm<T>);
				lua_setfield(L, -2, "__unm");
				lua_pushcfunction(L, &Vector::mul<T>);
				lua_setfield(L, -2, "__mul");
				return true;
			}
			return false;
		}
	} // end of Vector namespace

} // end of osgLua namespace

#endif
