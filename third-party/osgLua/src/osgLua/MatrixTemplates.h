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

#ifndef OSGLUA_MATRIXTEMPLATES
#define OSGLUA_MATRIXTEMPLATES

#include "MathValueTags.h"

#include "LuaIncludeFull.h"

#include <osgLua/introspection/Reflection>
#include <osgLua/introspection/ExtendedTypeInfo>
#include <osgLua/introspection/variant_cast>
#include <osgLua/introspection/Value>
#include <osgLua/introspection/Type>

#include <osg/Vec4>
#include <osg/Vec3>
#include <osg/Vec4f>
#include <osg/Vec3f>
#include <osg/Vec4d>
#include <osg/Vec3d>

#include <functional>

namespace osgLua {


	namespace detail {
		template<class T>
		T multMatrices(introspection::Value const& a, introspection::Value const& b) {
			return introspection::variant_cast<T>(a) * introspection::variant_cast<T>(b);
		}

		template<class Mat, class Vec>
		Vec xformVec(introspection::Value const& theMat, introspection::Value const& theVec) {
			return introspection::variant_cast<Mat>(theMat) * introspection::variant_cast<Vec>(theVec);
		}

	} // end of namespace detail


	namespace Matrix {
		template<class Mat, class Vec>
		bool xform(lua_State *L, Value *a, Value *b) {
			const introspection::Type &typeA = a->getType();
			const introspection::Type &typeB = b->getType();
			static const introspection::Type& vecType =
			    introspection::Reflection::getType(extended_typeid<Vec>());
			static const introspection::Type& matType =
			    introspection::Reflection::getType(extended_typeid<Mat>());
			if (typeA == matType && typeB == vecType) {
				introspection::Value ret = detail::xformVec<Mat, Vec>(a->get(), b->get());
				Value::push(L, ret);
				return true;
			}
			return false;
		}

		template<class T>
		int mul(lua_State *L) {
			Value *a = Value::getRequired(L, 1);
			Value *b = Value::getRequired(L, 2);

			const introspection::Type &typeA = a->getType();
			const introspection::Type &typeB = b->getType();

			static const introspection::Type& myType =
			    introspection::Reflection::getType(extended_typeid<T>());
			if (typeA == myType && typeB == myType) {
				/// matrix multiplication
				introspection::Value ret = detail::multMatrices<T>(a->get(), b->get());
				Value::push(L, ret);
				return 1;
			}
			if (typeA == myType) {
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

			return luaL_error(L, "[%s:%d] Could not multiply instances of %s, %s", __FILE__, __LINE__, typeA.getQualifiedName().c_str(), typeB.getQualifiedName().c_str());
		}

		/// Shared function template to handle both equality and less-than comparison.
		template<class T, typename FunctorType>
		int apply_binary_bool_predicate(lua_State *L, FunctorType predicate) {
			Value *a = Value::getRequired(L, 1);
			Value *b = Value::getRequired(L, 2);

			const introspection::Type &typeA = a->getType();
			const introspection::Type &typeB = b->getType();
			try {
				static const introspection::Type& myType =
				    introspection::Reflection::getType(extended_typeid<T>());
				if (typeA == myType && typeB == myType) {
					bool ret = predicate(introspection::variant_cast<T>(a->get()), introspection::variant_cast<T>(b->get()));
					lua_pushboolean(L, ret);
					return 1;
				} else {
					luaL_error(L, "[%s:%d] Could not compare instances of %s, %s, in comparator for %s", __FILE__, __LINE__,
					           typeA.getQualifiedName().c_str(),
					           typeB.getQualifiedName().c_str(),
					           myType.getQualifiedName().c_str());
				}
			} catch (std::exception & e) {
				luaL_error(L, "[%s:%d] Could not compare instances in comparator for %s - got exception %s", __FILE__, __LINE__, e.what());
			}

			return 0;
		}

		template<class T>
		int eq(lua_State *L) {
			return apply_binary_bool_predicate<T>(L, std::equal_to<T>());
		}

		template<class T>
		int lt(lua_State *L) {
			return apply_binary_bool_predicate<T>(L, std::less<T>());
		}

		template<class T>
		bool bind_metamethods(lua_State *L, const introspection::Type &valT) {
			try {
				static const introspection::Type& myType =
				    introspection::Reflection::getType(extended_typeid<T>());
				if (valT == myType) {
					detail::setMatrix(L);
					lua_pushcfunction(L, &Matrix::mul<T>);
					lua_setfield(L, -2, "__mul");
					/*
					lua_pushcfunction(L, &Matrix::eq<T>);
					lua_setfield(L, -2, "__eq");
					lua_pushcfunction(L, &Matrix::lt<T>);
					lua_setfield(L, -2, "__lt");
					*/
					return true;
				}
			} catch (introspection::Exception & e) {
				std::cerr << "Caught exception trying to bind metamethods for " << valT.getQualifiedName() << " - info: " << e.what() << std::endl;
				throw;
			}
			return false;
		}
	} // end of Matrix namespace

} // end of osgLua namespace

#endif
