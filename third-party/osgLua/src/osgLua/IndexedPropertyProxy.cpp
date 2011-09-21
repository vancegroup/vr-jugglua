/**
	@file
	@brief Implementation

	@date 2011

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

//          Copyright Iowa State University 2011.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// Internal Includes
#include "IndexedPropertyProxy.h"
#include "LuaIncludeFull.h"
#include <osgLua/Value>

// Library/third-party includes
// - none

// Standard includes
// - none

namespace osgLua {
	static const char INDEXED_PROPERTY_PROXY_REGISTRY_NAME[] = "osgLua.IndexedPropertyProxy";
	/*
		void IndexedPropertyProxy::addToRegistry(lua_State * L) {
			luaL_newmetatable(L, INDEXED_PROPERTY_PROXY_REGISTRY_NAME);
		}
		IndexedPropertyProxy * IndexedPropertyProxy::get(lua_State * L, int index) {
			void * ud = luaL_checkudata(L, index, INDEXED_PROPERTY_PROXY_REGISTRY_NAME);
			luaL_argcheck(L, ud != NULL, index, "osgLua IndexedPropertyProxy expected");
			return static_cast<IndexedPropertyProxy *>(ud);
		}

		IndexedPropertyProxy * IndexedPropertyProxy::pushNew(lua_State * L, introspection::Value & instance, const introspection::PropertyInfo * property) {
			IndexedPropertyProxy * p;
			void * ud = lua_newuserdata(L, sizeof(IndexedPropertyProxy));
			if (!ud) {
				throw std::bad_alloc();
			}
			try {
				p = new(ud) IndexedPropertyProxy(instance, property);
				return p;
			} catch (...) {
				lua_pop(L);
				return NULL;
			}
		}
	*/
	IndexedPropertyProxy::IndexedPropertyProxy(introspection::Value  const& instance, const introspection::PropertyInfo * property)
		: _instance(instance)
		, _propInfo(property)
	{}

	int IndexedPropertyProxy::index(lua_State *L) {
		if (lua_isstring(L, 1)) {
			lua_pushliteral(L, "insert");
			if (lua_equal(L, 1, -1)) {
				lua_pop(L, 1);
				lua_remove(L, 1);
				Base::NonConstInstanceMethod::pushInstanceMethod<&IndexedPropertyProxy::insert>(L);
				return lua_gettop(L);
			}
		} else if (lua_isnumber(L, 1)) {
			/// indexed property element access
			Value::push(L, _propInfo->getArrayItem(_instance, lua_tointeger(L, 1) - 1));
			return 1;
		}
		return luaL_error(L, "Indexed property %s expected access of element by index, starting at 1, or a method call to 'insert'", _propInfo->getName().c_str());
	}

	int IndexedPropertyProxy::newindex(lua_State *L) {
		int eltIndex = luaL_checkint(L, 1) - 1;
		_propInfo->setArrayItem(_instance, eltIndex, Value::getRequired(L, 2)->get());
		return 0;
	}

	int IndexedPropertyProxy::len(lua_State *L) {
		assert(_propInfo);
		lua_pushinteger(L, _propInfo->getNumArrayItems(_instance));
		return 1;
	}
	int IndexedPropertyProxy::insert(lua_State *L) {
		/// @todo
		return 0;
	}



	void IndexedPropertyProxy::registerAdditionalMetamethods(lua_State *L) {
		Base::NonConstInstanceMethod::registerMetamethod<&IndexedPropertyProxy::index>(L, "__index");
		Base::NonConstInstanceMethod::registerMetamethod<&IndexedPropertyProxy::newindex>(L, "__newindex");
		Base::NonConstInstanceMethod::registerMetamethod<&IndexedPropertyProxy::len>(L, "__len");
	}
}
