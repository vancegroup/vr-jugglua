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

#pragma once
#ifndef INCLUDED_osgLuaBind_ConverterBase_h_GUID_65295703_c834_491d_87ae_8941e9b1c61e
#define INCLUDED_osgLuaBind_ConverterBase_h_GUID_65295703_c834_491d_87ae_8941e9b1c61e

// Internal Includes
#include "osgLuaBind_verbose.h"
#include "osgLuaBind_getQualifiedName.h"

// Library/third-party includes
// - none

// Standard includes
// - none

namespace osgLuaBind {

	namespace detail {
		/// Base class for converting osg typed values/objects in C++/luabind to/from osgLua representations
		template<typename ValueType, typename ContainerType = T>
		struct ConverterBase : ::luabind::native_converter_base<ContainerType> {
			typedef ValueType value_t;

			typedef ContainerType container_t;

			/// Compute type matching score for signature matching and overload resolution
			static int compute_score(lua_State* L, int index) {
				osgLua::Value * v = osgLua::Value::get(L, index);
				if (!v) {
					OSGLUABIND_VERBOSE("Not a osgLua object");
					return -1;
				}
				static const osgLua::introspection::Type& destType =
				    osgLua::introspection::Reflection::getType(extended_typeid<T>());
				const osgLua::introspection::Type& type = v->get().getType();
				OSGLUABIND_VERBOSE("Source type: " << getQualifiedName(v->get()));
				OSGLUABIND_VERBOSE("Target type: " << getQualifiedName<T>());
				try {
					if (type == destType) {
						OSGLUABIND_VERBOSE("Exact match for type!");
						return 2;
					} else {
						return osglua_casting_score_for_type<T>::get(v);
					}
				} catch (...) {
					/// @todo make this catch only osgIntrospection exceptions
					return -1;
				}
				return -1;
			}

			/// Accept in C++ from Lua
			value_t from(lua_State* L, int index) {
				osgLua::Value * v = osgLua::Value::get(L, index);
				if (!v) {
					return value_t();
				}
				return osgLua::introspection::variant_cast<value_t>(v->get());
			}

			/// Transition C++ to Lua
			void to(lua_State* L, value_t const& x) {
				osgLua::Value::push(L, x);
			}
		};
	} // end of namespace detail

} // end of namespace osgLuaBind

#endif // INCLUDED_osgLuaBind_ConverterBase_h_GUID_65295703_c834_491d_87ae_8941e9b1c61e
