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
#ifndef INCLUDED_osgLuaBind_castingScoreForType_h_GUID_842edffe_8490_4967_bb7c_30ccee14d36c
#define INCLUDED_osgLuaBind_castingScoreForType_h_GUID_842edffe_8490_4967_bb7c_30ccee14d36c

// Internal Includes
#include "osgLuaBind_isRefType.h"
#include "osgLuaBind_isValueType.h"

// Library/third-party includes
#include <osgLua/introspection/Exceptions>
#include <osgLua/introspection/Value>
#include <osgLua/introspection/variant_cast>

// Standard includes
// - none

namespace osgLuaBind {

	namespace detail {

		/// Type-dispatched function for determine the score possible by casting.

		template <typename T, typename Enable = void>
		struct castingScoreForType;

		/// Version for value types.
		template <typename T>
		struct castingScoreForType<T, typename IsValueType<T>::type> {
			typedef T target_t;

			static int get(osgLua::introspection::Value const& v) {
				try {
					osgLua::introspection::variant_cast<target_t>(v);
				} catch (osgLua::introspection::Exception &) {
					return -1;
				}
				OSGLUABIND_VERBOSE("Convertible match for type.");
				return 0;
			}
		};

		/// Partial specialization of casting score computation for referenced object types
		template <typename T>
		struct castingScoreForType<T, typename IsRefType<T>::type> {
			typedef T* target_t;

			static int get(osgLua::introspection::Value const& v) {
				if (osgLua::introspection::variant_cast<target_t>(v) != NULL) {
					if (osgLua::introspection::requires_conversion<target_t>(v)) {
						OSGLUABIND_VERBOSE("Convertible match for type.");
						return 0;
					} else {
						OSGLUABIND_VERBOSE("Polymorphic match for type.");
						return 1;
					}
				}
				return -1;
			}
		};
		template <typename T>
		struct castingScoreForType<T*, typename IsRefType<T>::type> : castingScoreForType<T, typename IsRefType<T>::type> {};

		template <typename T>
		struct castingScoreForType<osg::ref_ptr<T>, typename IsRefType<T>::type> : castingScoreForType<T, typename IsRefType<T>::type> {};
	} // end of namespace detail

} // end of namespace osgLuaBind

#endif // INCLUDED_osgLuaBind_castingScoreForType_h_GUID_842edffe_8490_4967_bb7c_30ccee14d36c
