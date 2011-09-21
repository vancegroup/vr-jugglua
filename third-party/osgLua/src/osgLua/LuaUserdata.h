/** @file
	@brief Header

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

#pragma once
#ifndef INCLUDED_LuaUserdata_h_GUID_52d4ca22_cd5a_420e_b6fd_f46db9eb1f93
#define INCLUDED_LuaUserdata_h_GUID_52d4ca22_cd5a_420e_b6fd_f46db9eb1f93

// Internal Includes
#include "LuaIncludeFull.h"

// Library/third-party includes
// - none

// Standard includes
#include <typeinfo>
#include <cassert>

namespace osgLua {
	class LuaUserdataBase {
		public:
			static const char LUA_INDEX_METAMETHOD_NAME[] = "__index";
	};
	template<typename Derived>
	class LuaUserdata : public LuaUserdataBase {
		public:
			typedef int (Derived::*NonConstInstanceMethodPtrType)(lua_State *);
			typedef int (Derived::*ConstInstanceMethodPtrType)(lua_State *) const;
			typedef Derived * PointerToDerivedType;
			typedef Derived DerivedType;

		private:
			static const size_t ALLOCATION_SIZE = sizeof(Derived);

			static const char * _getRegistryString() {
				return typeid(Derived).name();
			}

			static void _pushMetatable(lua_State * L) {
				luaL_getmetatable(L, _getRegistryString());
			}

			static void _pushIndexMetatable(lua_State * L) {
				_pushMetatable(L);
				int metatableIndex = lua_gettop(L);
				lua_getfield(L, metatableIndex, LUA_INDEX_METAMETHOD_NAME);
				if (lua_isnil(L, -1)) {
					lua_pop(L, 1);
					lua_newtable(L);
					lua_pushvalue(L, -1); /// now the top two values are the __index table
					lua_setfield(L, metatableIndex, LUA_INDEX_METAMETHOD_NAME);
				} else if (!lua_istable(L, -1)) {
					luaL_error(L, "While registering an instance method of %s: __index metamethod already set to value of type %s!", _getRegistryString(), lua_typename(L, -1));
					return;
				}
				/// OK, past this point we have the __index table at -1.

				/// Remove the metatable from the stack so that all we have pushed is the __index table
				lua_remove(L, -2);
			}
			/*
						static int _gc(lua_State * L) {
							PointerType instance = lua_checkudata(L, 1, _getRegistryString());
							assert(instance);
							/// explicitly call destructor before Lua deletes the memory.
							instance->~Derived();
						}
			*/
		protected:
			template<typename PtrToMemberFuncType>
			class InstanceMethodHandler {
				private:

					template<PtrToMemberFuncType M>
					static int _callInstanceMethod(lua_State * L) {
						PointerToDerivedType instance = lua_checkudata(L, 1, _getRegistryString());
						if (!instance) {
							return luaL_error(L, "Trying to call an instance method of %s, but first argument is not an instance!", _getRegistryString());
						}
						lua_remove(L, 1); /// remove the instance from the stack, since it's been handled.
						return ((*instance).*(M))(L);
					}

					template<PtrToMemberFuncType M>
					static void _pushInstanceMethod(lua_State * L) {
						lua_pushcfunction(L, &_callInstanceMethod<M>);
					}

				public:

					template<PtrToMemberFuncType M>
					static void registerMetamethod(lua_State * L, const char * metamethodName) {
						_pushMetatable(L, _getRegistryString());
						_pushInstanceMethod<M>(L);
						lua_setfield(L, -2, metamethodName); /// table is one below the top of the stack
						lua_pop(L, 1); /// pop the metatable off the stack.
					}

					template<PtrToMemberFuncType M>
					static void registerObjectMethod(lua_State * L, const char * methodName) {
						_pushIndexMetatable(L);
						_pushInstanceMethod<M>(L);
						lua_setfield(L, -2, methodName); /// table is one below the top of the stack
						lua_pop(L, 1); /// pop the __index table off the stack.
					}
			};

			typedef InstanceMethodHandler<NonConstInstanceMethodPtrType> NonConstInstanceMethod;
			typedef InstanceMethodHandler<ConstInstanceMethodPtrType> ConstInstanceMethod;

			static void * allocate(lua_State * L) {
				void * ud = lua_newuserdata(L, ALLOCATION_SIZE);
				if (!ud) {
					throw std::bad_alloc();
				}
			}
			static PointerToDerivedType pushNewWithLuaParam(lua_State * L) {
				void * ud = allocate(L);
				PointerToDerivedType p;
				try {
					p = new(ud) DerivedType(L);
					return p;
				} catch (...) {
					lua_pop(L);
					return NULL;
				}
			}

			static PointerToDerivedType pushNew(lua_State * L) {
				void * ud = allocate(L);
				PointerToDerivedType p;
				try {
					p = new(ud) DerivedType();
					return p;
				} catch (...) {
					lua_pop(L);
					return NULL;
				}
			}

			template<typename T1>
			static PointerToDerivedType pushNew(lua_State * L, T1 a1) {
				void * ud = allocate(L);
				PointerToDerivedType p;
				try {
					p = new(ud) DerivedType(a1);
					return p;
				} catch (...) {
					lua_pop(L);
					return NULL;
				}
			}

			template<typename T1, typename T2>
			static PointerToDerivedType pushNew(lua_State * L, T1 a1, T2 a2) {
				void * ud = allocate(L);
				PointerToDerivedType p;
				try {
					p = new(ud) DerivedType(a1, a2);
					return p;
				} catch (...) {
					lua_pop(L);
					return NULL;
				}
			}

			template<typename T1, typename T2, typename T3>
			static PointerToDerivedType pushNew(lua_State * L, T1 a1, T2 a2, T3 a3) {
				void * ud = allocate(L);
				PointerToDerivedType p;
				try {
					p = new(ud) DerivedType(a1, a2, a3);
					return p;
				} catch (...) {
					lua_pop(L);
					return NULL;
				}
			}

			static void pushCreatorFunction(lua_State * L) {
				lua_pushcfunction(L, pushNewWithLuaParam);
			}

			/// Implement your own method to override this if you want to
			/// register more than just __gc. You can call registerNonConstInstanceMetamethod()
			/// and registerConstInstanceMetamethod() from within your method.
			static void registerAdditionalMetamethods(lua_State *) {}

		public:
			static void createMetatable(lua_State * L) {
				if (luaL_newmetatable(L, _getRegistryString())) {
					NonConstInstanceMethod::registerMetamethod < Derived::~Derived > (L, "__gc");
					registerAdditionalMetamethods(L);
				}
			}


	};


}

#endif // INCLUDED_LuaUserdata_h_GUID_52d4ca22_cd5a_420e_b6fd_f46db9eb1f93
