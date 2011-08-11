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

//          Copyright Iowa State University 2010-2011.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#pragma once
#ifndef INCLUDED_osgLuaBind_ConverterBase_h_GUID_65295703_c834_491d_87ae_8941e9b1c61e
#define INCLUDED_osgLuaBind_ConverterBase_h_GUID_65295703_c834_491d_87ae_8941e9b1c61e

// Internal Includes
#include "osgLuaBind_verbose.h"
#include "osgLuaBind_getQualifiedName.h"
#include "osgLuaBind_castingScoreForType.h"
#include "osgLuaBind_unwrapPtr.h"
#include "osgLuaBind_isSameType.h"

// Library/third-party includes
#include <luabind/lua_state_fwd.hpp>
#include <osgLua/Value>
#include <osgLua/introspection/variant_cast>

// Standard includes
// - none

namespace osgLuaBind {

	namespace detail {
		/// Base class for converting osg typed values/objects in C++/luabind to/from osgLua representations
		template<typename T, typename ContainedType = T>
		struct ConverterBase : ::luabind::native_converter_base<T> {
			typedef ContainedType osg_t;

			typedef T container_t;

			//typedef typename unwrapPtr<container_t>::return_t from_return_t;

			/// Compute type matching score for signature matching and overload resolution
			static int compute_score(lua_State* L, int index) {
				osgLua::Value * v = osgLua::Value::get(L, index);
				if (!v) {
					OSGLUABIND_VERBOSE("Not a osgLua object");
					return -1;
				}
				if (isSameType<container_t>(v->get())) {
					OSGLUABIND_VERBOSE("Exact match for type!");
					return 2;
				}
				return castingScoreForType<container_t>::get(v->get());
			}

			/// Accept in C++ from Lua
			container_t from(lua_State* L, int index) {
				osgLua::Value * v = osgLua::Value::get(L, index);
				if (!v) {
					return container_t();
				}
				return osgLua::introspection::variant_cast<container_t>(v->get());
			}

			/// Transition C++ to Lua
			void to(lua_State* L, container_t const& x) {
				osgLua::Value::push(L, unwrapPtr<container_t>::apply(x));
			}
		};
	} // end of namespace detail

} // end of namespace osgLuaBind

#endif // INCLUDED_osgLuaBind_ConverterBase_h_GUID_65295703_c834_491d_87ae_8941e9b1c61e
