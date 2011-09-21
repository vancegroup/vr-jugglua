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

// Library/third-party includes
// - none

// Standard includes
#include <stdexcept>

namespace osgLua {
	static const char INDEXED_PROPERTY_PROXY_REGISTRY_NAME[] = "osgLua.IndexedPropertyProxy";

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

	IndexedPropertyProxy::IndexedPropertyProxy(introspection::Value & instance, const introspection::PropertyInfo * property)
		: _instance(&instance)
		, _propInfo(property)
	{}

	int IndexedPropertyProxy::index(lua_State *L, int);
	int IndexedPropertyProxy::newindex(lua_State *L);
	int IndexedPropertyProxy::insert(lua_State *L);

	int IndexedPropertyProxy::_index(lua_State *L) {
		get(L, 1)->index(L);
	}
	int IndexedPropertyProxy::_newindex(lua_State *L) {
		get(L, 1)->newindex(L);

	}

}
