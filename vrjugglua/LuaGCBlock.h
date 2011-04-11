/** @file LuaGCBlock.h
	@brief

	@date 2011

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

#pragma once
#ifndef INCLUDED_LuaGCBlock_h_GUID_8c906cef_6f6d_49c4_b436_f774451856d1
#define INCLUDED_LuaGCBlock_h_GUID_8c906cef_6f6d_49c4_b436_f774451856d1

// Internal Includes
#include <vrjugglua/LuaInclude.h>

// Library/third-party includes
// - none

// Standard includes
#include <stdexcept>

namespace vrjLua {

/// Class providing RAII-style method of disabling the Lua garbage collector,
/// for example, during library loading. Disables garbage collection
	class LuaGCBlock {
		public:
			/// Derived exception class thrown by constructor
			struct NullStateError : public std::runtime_error {
				NullStateError() : std::runtime_error("Null Lua state passed to LuaGCBlock constructor!") {}
			};

			/** @brief Constructor: disables garbage collection in the given state.
			
				Requires a valid state pointer.
			
				@throws NullStateError if state pointer given is NULL
			*/
			LuaGCBlock(lua_State * L) : _L(L) {
				if (!_L) {
					throw NullStateError();
				}
				lua_gc(_L, LUA_GCSTOP, 0);
			}

			/// Destructor: restarts garbage collection on the lua state.
			~LuaGCBlock() {
				lua_gc(_L, LUA_GCRESTART, 0);
			}
			
			/// Accessor to the GC-blocked state
			lua_State * state() const {
				return _L;
			}


		private:
			lua_State * _L;

	};

}

#endif // INCLUDED_LuaGCBlock_h_GUID_8c906cef_6f6d_49c4_b436_f774451856d1
