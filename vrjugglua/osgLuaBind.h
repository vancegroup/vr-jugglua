/**	@file	osgLuaBind.h
	@brief	header with template specializations providing Luabind-compatible
		conversions to/from osgLua values

	@date
	2009-2011

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
#ifndef INCLUDED_vrjugglua_osgLuaBind_h
#define INCLUDED_vrjugglua_osgLuaBind_h

// Local includes
// - none

// Library/third-party includes
#include <luabind/luabind.hpp>

#include <osg/Object>
#include <osg/ref_ptr>
#include <osgLua/Value>
#include <osgLua/Introspection_variant_cast>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_base_of.hpp>

// Standard includes
#ifdef BUILD_OSGLUABIND_VERBOSE
#include <iostream>
#define OSGLUABIND_VERBOSE(X) std::cout << "[osgLuaBind.h:" << __LINE__ << "] " << X << std::endl
#else
#define OSGLUABIND_VERBOSE(X)
#endif

// lua_State is a commonly-passed incomplete type: let Boost know it's never derived from anything.
#include <boost/type_traits/detail/bool_trait_def.hpp>
namespace boost {
BOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC2_1(typename T,is_base_and_derived,T,lua_State,false)
}

namespace luabind {
	namespace detail {
		// Convenience typedef for the root of the OSG referenced object class hierarchy.
		typedef ::osg::Object osg_ref_base_t;

		/// Function template to "unwrap" a pointer into another pointer
		template <typename RetT, typename ClassT>
		static RetT unwrap_osg_ptr(ClassT * const &ptr) {
			return ptr;
		}

		/// Function template to "unwrap" a ref_ptr into a raw pointer
		template <typename RetT, typename ClassT>
		static RetT unwrap_osg_ptr(osg::ref_ptr<ClassT> const & ptr) {
			return ptr.get();
		}

		/// Type-dispatched function for determine the score possible by casting.
		/// Default version is for value types.
		template <typename T>
		struct osglua_casting_score_for_type {
			typedef T target_t;

			static int get(osgLua::Value * v) {
				try {
					osgIntrospection::variant_cast<target_t>(v->get());
				} catch (...) {
					/// @todo make this catch only osgIntrospection exceptions
					return -1;
				}
				OSGLUABIND_VERBOSE("Convertible match for type.");
				return 0;
			}
		};

		/// Partial specialization of casting score computation for referenced object types
		template <typename T>
		struct osglua_casting_score_for_type<T*> {
			typedef T* target_t;

			static int get(osgLua::Value * v) {
				if (osgIntrospection::variant_cast<target_t>(v->get()) != NULL) {
					if (osgIntrospection::requires_conversion<target_t>(v->get())) {
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

		/// Template function for shared score computation (both value and reference) -
		/// uses a type-dispatched partial specialized function for the "inner section"
		/// that differs between value and reference.
		template <typename T>
		static int compute_osglua_score_for_type(lua_State* L, int index) {

			osgLua::Value * v = osgLua::Value::get(L, index);
			if (!v) {
				OSGLUABIND_VERBOSE( "Not a osgLua object" );
				return -1;
			}
			static const osgIntrospection::Type& destType =
			    osgIntrospection::Reflection::getType(extended_typeid<T>());
			const osgIntrospection::Type& type = v->get().getType();
			OSGLUABIND_VERBOSE( "Source type: " << v->get().getType().getQualifiedName());
			OSGLUABIND_VERBOSE( "Target type: " << destType.getQualifiedName());
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

		/// Traits template for determining the luabind passing style type based on
		/// the type of container (raw pointer or ref_ptr)
		template <typename T>
		struct osglua_passing_style;

		template <typename T>
		struct osglua_passing_style<T*> {
			typedef detail::by_pointer<T> type;
		};

		template <typename T>
		struct osglua_passing_style< ::osg::ref_ptr<T> > {
			typedef detail::by_value< ::osg::ref_ptr<T> > type;
		};
	} // end of namespace detail

	/// Base class for converting osg referenced types in C++/luabind to/from osgLua values
	template<typename OSG_QUALIFIED_TYPENAME, typename CONTAINER_TYPENAME = OSG_QUALIFIED_TYPENAME*>
	struct osglua_ref_converter_base : native_converter_base<CONTAINER_TYPENAME> {
		typedef OSG_QUALIFIED_TYPENAME* raw_ptr_t;
		typedef osg::ref_ptr<OSG_QUALIFIED_TYPENAME> ref_ptr_t;
		typedef CONTAINER_TYPENAME container_t;

		/// Compute type matching score for signature matching and overload resolution
		static int compute_score(lua_State* L, int index) {
			return detail::compute_osglua_score_for_type<raw_ptr_t>(L, index);
		}

		/// Accept in C++ from Lua
		raw_ptr_t from(lua_State* L, int index) {
			osgLua::Value * v = osgLua::Value::get(L, index);
			if (!v) {
				return NULL;
			}
			return osgIntrospection::variant_cast<raw_ptr_t>(v->get());
		}

		/// Transition C++ to Lua
		void to(lua_State* L, container_t const& x) {
			osgLua::Value::push(L, detail::unwrap_osg_ptr<raw_ptr_t>(x));
		}


		/// @name Extra required methods for converter
		/// I think these are not provided by the native_converter_base in luabind because that
		/// library assumes that you won't make a converter for pointer types. These are minimal
		/// forwarding functions in any case.
		/// @{
		raw_ptr_t apply(lua_State* L, typename detail::osglua_passing_style<container_t>::type &, int index) {
			return from(L, index);
		}

		void apply(lua_State* L, container_t const& value) {
			to(L, value);
		}

		int match(lua_State* L, typename detail::osglua_passing_style<container_t>::type, int index) {
			return compute_score(L, index);
		}
		/// @}
	};

	/// Base class for converting osg value types in C++/luabind to/from osgLua values
	template<typename OSG_QUALIFIED_TYPENAME>
	struct osglua_val_converter_base : native_converter_base<OSG_QUALIFIED_TYPENAME> {
		typedef OSG_QUALIFIED_TYPENAME value_t;

		/// Compute type matching score for signature matching and overload resolution
		static int compute_score(lua_State* L, int index) {
			return detail::compute_osglua_score_for_type<value_t>(L, index);
		}

		/// Accept in C++ from Lua
		value_t from(lua_State* L, int index) {
			osgLua::Value * v = osgLua::Value::get(L, index);
			if (!v) {
				return value_t();
			}
			return osgIntrospection::variant_cast<value_t>(v->get());
		}

		/// Transition C++ to Lua
		void to(lua_State* L, value_t const& x) {
			osgLua::Value::push(L, x);
		}
	};

}


//-- Reference Types --//
namespace luabind {

	/// Raw pointers to objects inheriting from luabind::detail::osg_ref_base_t
	/// should be converted as osgLua reference objects
	template <typename T>
	struct default_converter< T*,
		typename boost::enable_if<
			typename boost::is_base_of< detail::osg_ref_base_t, T>::type
		>::type >
	: osglua_ref_converter_base<T>
	{};

	/// Anything held in an osg::ref_ptr should be converted as an osgLua reference object
	template <typename T>
	struct default_converter< ::osg::ref_ptr<T> >
	: osglua_ref_converter_base<T, ::osg::ref_ptr<T> >
	{};

	namespace detail {
		/// Shared template function for looking up a type name string in osgIntrospection
		/// and pusing it onto the lua stack.
		template <typename T>
		struct osglua_type_to_string
		{
			static void get(lua_State *L)
			{
				static const osgIntrospection::Type& destType =
					osgIntrospection::Reflection::getType(extended_typeid<T>());
				lua_pushstring(L, destType.getQualifiedName().c_str());
			}
		};

		/// type_to_string partial specialization for types inheriting from luabind::detail::osg_ref_base_t
		/// includes annotation mentioning osgLuaBind and type name from osgIntrospection.
		///
		/// @note Luabind includes forwarding partial specializations to apply modifiers like *, &,
		/// and const, so no need to define those here.
		template <typename T>
		struct type_to_string<T,
			typename boost::enable_if<
				typename boost::is_base_of< detail::osg_ref_base_t, T>::type
			>::type
		>
		{
			static void get(lua_State *L)
			{
				lua_pushstring(L, "[osgLuaBind object] ");
				osglua_type_to_string<T>::get(L);
				lua_concat(L, 2);
			}
		};


		/// type_to_string partial specialization for types held in an osg::ref_ptr
		/// includes annotation mentioning osgLuaBind and type name from osgIntrospection,
		/// wrapped in osg::ref_ptr< and >.
		///
		/// @note Luabind includes forwarding partial specializations to apply modifiers like *, &,
		/// and const, so no need to define those here.
		template <typename T>
		struct type_to_string< osg::ref_ptr<T> >
		{
			static void get(lua_State* L)
			{
				lua_pushstring(L, "[osgLuaBind object] osg::ref_ptr<");
				osglua_type_to_string<T>::get(L);
				lua_pushstring(L, ">");
				lua_concat(L, 3);
			}
		};
	}
}
//-- Value Types --//

namespace luabind {
	/// Base class for "true" osg type traits.
	struct OSGTraitTrue {
		static const bool value = true;
		static const bool truevalue = true;
		typedef void type;
	};

	/// Templated type trait: all types are assumed to
	/// not be OSG value types unless otherwise stated.
	template <typename T>
	struct IsOSGValue {
		static const bool value = false;
	};

	/// Types flagged as being OSG value types should be converted
	/// with the osglua value converter.
	template <typename T>
	struct default_converter< T,
		typename boost::enable_if_c<
			IsOSGValue<T>::value
		>::type
	>
	: osglua_val_converter_base<T>
	{};
	namespace detail {

		/// Types flagged as being OSG value types get a human-readable
		/// name string from osgIntrospection with an annotation mentioning
		/// osgLuaBind.
		template <typename T>
		struct type_to_string<T,
			typename boost::enable_if_c<
				IsOSGValue<T>::value
			>::type
		>
		{
			static void get(lua_State *L)
			{
				lua_pushstring(L, "[osgLuaBind value] ");
				osglua_type_to_string<T>::get(L);
				lua_concat(L, 2);
			}
		};
	}
}

namespace boost {
	/// Anything defined as an OSG Value does not inherit from the osg base referenced object type
	BOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC2_1(typename T, is_base_and_derived, ::luabind::detail::osg_ref_base_t, T, luabind::IsOSGValue<T>::truevalue)
}

#include <boost/type_traits/detail/bool_trait_undef.hpp>

#ifndef CREATE_OSGLUA_VALUE_CONVERTER

/// Macro to define traits to indicate to Luabind that a type is an osg value type.
/// If you're binding a function/method/property that involves an OSG value type
/// not already mentioned at the bottom of this header, you'll need to use this
/// macro in your source file to tag the type you're using as an OSG value type.
/// Failing to do so will give you a weird error at runtime when trying to use that
/// bound item that reads something like "No matching overload found, candidates:
/// void functionname(custom [Z7osg...])"
#define CREATE_OSGLUA_VALUE_CONVERTER(T) \
	namespace luabind { \
		template <> \
		struct IsOSGValue< T > \
		  : OSGTraitTrue \
		{}; \
		\
	}

/// Internal-use macro to forward declare and create value converter in one step.
#define FORWARD_DECL_AND_CREATE_VALUE_CONVERTER(NS, T) \
	namespace NS { \
		class T; \
	} \
	CREATE_OSGLUA_VALUE_CONVERTER( NS :: T )

/// @name Forward declaration and type trait tagging of common OSG value types.
///
/// To bind something using one of these types, you'll still need to include that type's
/// header, but forward declaration here keeps this header from forcing every file
/// using it to include lots of extra headers.
///
/// Any value types may be added here to make their converter available automatically when
/// including this file and the corresponding osg header. To make a converter available
/// without modifying this main header, use the CREATE_OSGLUA_VALUE_CONVERTER macro.
///
/// @{

FORWARD_DECL_AND_CREATE_VALUE_CONVERTER(osg, Matrixd);
FORWARD_DECL_AND_CREATE_VALUE_CONVERTER(osg, Matrixf);
FORWARD_DECL_AND_CREATE_VALUE_CONVERTER(osg, Vec3d);
FORWARD_DECL_AND_CREATE_VALUE_CONVERTER(osg, Vec3f);
FORWARD_DECL_AND_CREATE_VALUE_CONVERTER(osg, Vec4d);
FORWARD_DECL_AND_CREATE_VALUE_CONVERTER(osg, Vec4f);
FORWARD_DECL_AND_CREATE_VALUE_CONVERTER(osg, Quat);

/// @}

// Sorry, you probably shouldn't use this macro in your own files.
#undef FORWARD_DECL_AND_CREATE_VALUE_CONVERTER


#endif // ifndef CREATE_OSGLUA_VALUE_CONVERTER

#endif // INCLUDED_vrjugglua_osgLuaBind_h
