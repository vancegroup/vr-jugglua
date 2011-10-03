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
#include <typeinfo>

namespace osgLua {

	IndexedPropertyProxy::IndexedPropertyProxy(introspection::Value  const& instance, const introspection::PropertyInfo * property)
		: _instance(instance)
		, _propInfo(property)
	{}

	const char * IndexedPropertyProxy::_getMethodRegistryString() {
		static std::string regString = std::string(typeid(IndexedPropertyProxy).name()) + ":MethodTable";
		return regString.c_str();
	}

	bool IndexedPropertyProxy::_pushItemAtArrayIndex(lua_State *L, int i) {
		if (i < _propInfo->getNumArrayItems(_instance) && i >= 0) {
			Value::push(L, _propInfo->getArrayItem(_instance, i));
			return true;
		}
		return false; // in case of out of range
	}

	int IndexedPropertyProxy::index(lua_State *L) {
		int argType = lua_type(L, 2);
		if (argType == LUA_TSTRING) {
			int top = lua_gettop(L);
			luaL_getmetatable(L, _getMethodRegistryString());
			lua_pushvalue(L, 2);
			lua_gettable(L, 3);
			if (!lua_isnil(L, -1)) {
				return 1;
			}
			lua_settop(L, top);
		} else if (argType == LUA_TNUMBER) {
			/// indexed property element access
			int i = lua_tointeger(L, 2) - 1;
			if (_pushItemAtArrayIndex(L, i)) {
				return 1;
			}
		}

		//lua_pushnil(L);
		//return 1;
		return luaL_error(L, "Indexed property %s expected access of element by index, in 1 through %d, or a method call to 'insert'", _propInfo->getName().c_str(), _propInfo->getNumArrayItems(_instance));
	}

	int IndexedPropertyProxy::newindex(lua_State *L) {
		const int eltIndex = luaL_checkint(L, 2) - 1;
		const int n = _propInfo->getNumArrayItems(_instance);
		introspection::Value newval = getValue(L, 3);

		/// handle the "modify existing array element" case
		if (eltIndex < n && eltIndex >= 0) {
			try {
				_propInfo->setArrayItem(_instance, eltIndex, newval);
				return 0;
			} catch (introspection::Exception & e) {
				return luaL_error(L, "[%s:%d] Can't assign given value to index %d: got OSG exception '%s'", __FILE__, __LINE__, eltIndex + 1, e.what().c_str());
			}
		}

		/// handle the "append new array element" case
		if (eltIndex == n) {
			try {
				/// actually an insert, at the end of the array
				_propInfo->addArrayItem(_instance, newval);
				return 0;
			} catch (introspection::Exception & e) {
				return luaL_error(L, "[%s:%d] Can't append given value at the end of array (index %d): got OSG exception '%s'", __FILE__, __LINE__, eltIndex + 1, e.what().c_str());
			}
		}

		/// else, we've fallen through to an error

		if (n == 0) {
			return luaL_error(L, "[%s:%d] Can't assign to index %d: array has 0 elements, and not assigning to index 1", __FILE__, __LINE__, eltIndex + 1);
		}
		return luaL_error(L, "[%s:%d] Can't assign to index %d: out of existing range 1-%d, and not contiguous at end location %d",
		                  __FILE__,
		                  __LINE__,
		                  eltIndex + 1,
		                  n,
		                  n + 1);
	}

	int IndexedPropertyProxy::len(lua_State *L) {
		assert(_propInfo);
		lua_pushinteger(L, _propInfo->getNumArrayItems(_instance));
		return 1;
	}

	int IndexedPropertyProxy::insert(lua_State *L) {
		int numItems = _propInfo->getNumArrayItems(_instance);
		int location = -1;
		bool atEnd = true;
		introspection::Value newval;
		switch (lua_gettop(L)) {
			case 2:
				newval = getValue(L, 2);
				break;
			case 3:
				location = luaL_checkint(L, 2) - 1;
				newval = getValue(L, 3);
				atEnd = (location == numItems);
				break;
			default:
				return luaL_error(L, "Indexed property %s call to 'insert' expected at least a value, and potentially a location and a value", _propInfo->getName().c_str());
		}

		if (atEnd) {
			_propInfo->addArrayItem(_instance, newval);
		} else {
			_propInfo->insertArrayItem(_instance, location, newval);
		}

		return 0;
	}

	int IndexedPropertyProxy::remove(lua_State *L) {
		/// Default to removing the last item.
		int i = luaL_optinteger(L, 2, _propInfo->getNumArrayItems(_instance)) - 1;
		if (_pushItemAtArrayIndex(L, i)) {
			/// OK, this is in range, so we now have to delete it.
			_propInfo->removeArrayItem(_instance, i);
			return 1;
		}
		/// If we get down here, we were out of range.
		return luaL_error(L, "Can't remove item %d from array property %s: out of range. (Contains %d elements, starting at 1)", i = 1, _propInfo->getName().c_str(), _propInfo->getNumArrayItems(_instance));
	}

	void IndexedPropertyProxy::registerAdditionalMetamethods(lua_State *L) {
		Base::NonConstInstanceMethod::registerMetamethod<&IndexedPropertyProxy::index>(L, "__index");
		Base::NonConstInstanceMethod::registerMetamethod<&IndexedPropertyProxy::newindex>(L, "__newindex");
		Base::NonConstInstanceMethod::registerMetamethod<&IndexedPropertyProxy::len>(L, "__len");
		luaL_newmetatable(L, _getMethodRegistryString());
		Base::NonConstInstanceMethod::pushInstanceMethod<&IndexedPropertyProxy::insert>(L);
		lua_setfield(L, -2, "insert");
		Base::NonConstInstanceMethod::pushInstanceMethod<&IndexedPropertyProxy::remove>(L);
		lua_setfield(L, -2, "remove");
	}
}
