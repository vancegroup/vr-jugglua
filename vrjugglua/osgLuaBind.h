/**	@file	osgLuaBind.h
	@brief	header with template specializations providing Luabind-compatible
		conversions to/from osgLua values

	@date
	2009-2010

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/
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
#ifdef BUILD_VERBOSE
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
		template <typename RetT, typename ClassT>
		static RetT unwrap_osg_ptr(ClassT * const &ptr) {
			return ptr;
		}

		template <typename RetT, typename ClassT>
		static RetT unwrap_osg_ptr(osg::ref_ptr<ClassT> const & ptr) {
			return ptr.get();
		}

		template <typename T>
		struct osglua_casting_score_for_type {
			typedef T target_t;
			static int get(osgLua::Value * v) {
				try {
					osgIntrospection::variant_cast<target_t>(v->get());
				} catch (...) {
					return -1;
				}
				OSGLUABIND_VERBOSE("Convertible match for type.");
				return 0;
			}
		};

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

		template <typename T>
		static int compute_osglua_score_for_type(lua_State* L, int index) {

			osgLua::Value * v = osgLua::Value::get(L, index);
			if (!v) {
				OSGLUABIND_VERBOSE( "Not a osgLua value" );
				return -1;
			}
			static const osgIntrospection::Type& destType =
			    osgIntrospection::Reflection::getType(extended_typeid<T>());
			const osgIntrospection::Type& type = v->get().getType();
			OSGLUABIND_VERBOSE( "Destination type: " << destType.getQualifiedName());
			OSGLUABIND_VERBOSE( "Value type: " << v->get().getType().getQualifiedName());
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
	} // end of namespace detail

	/// Base class for converting osg referenced types to/from osgLua values
	template<typename OSG_QUALIFIED_TYPENAME, typename CONTAINER_TYPENAME = OSG_QUALIFIED_TYPENAME*>
	struct osglua_ref_converter_base : native_converter_base<CONTAINER_TYPENAME> {
		typedef OSG_QUALIFIED_TYPENAME* raw_ptr_t;
		typedef osg::ref_ptr<OSG_QUALIFIED_TYPENAME> ref_ptr_t;
		typedef CONTAINER_TYPENAME container_t;
		static int compute_score(lua_State* L, int index) {
			return detail::compute_osglua_score_for_type<raw_ptr_t>(L, index);
		}

		int match(lua_State* L, detail::by_pointer<OSG_QUALIFIED_TYPENAME>, int index) {
			return compute_score(L, index);
		}

		int match(lua_State* L, detail::by_value<ref_ptr_t>&, int index) {
			return compute_score(L, index);
		}

		raw_ptr_t from(lua_State* L, int index) {
			osgLua::Value * v = osgLua::Value::get(L, index);
			if (!v) {
				return NULL;
			}
			return osgIntrospection::variant_cast<raw_ptr_t>(v->get());
		}

		raw_ptr_t apply(lua_State* L, detail::by_pointer<OSG_QUALIFIED_TYPENAME>, int index) {
			return from(L, index);
		}

		ref_ptr_t apply(lua_State* L, detail::by_value<ref_ptr_t>&, int index) {
			return from(L, index);
		}

		void apply(lua_State* L, container_t const& value) {
			to(L, value);
		}

		void to(lua_State* L, container_t const& x) {
			osgLua::Value::push(L, detail::unwrap_osg_ptr<raw_ptr_t>(x));
		}
	};

	/// Base class for converting osg value types to/from osgLua values
	template<typename OSG_QUALIFIED_TYPENAME>
	struct osglua_val_converter_base : native_converter_base<OSG_QUALIFIED_TYPENAME> {
		static int compute_score(lua_State* L, int index) {
			return detail::compute_osglua_score_for_type<OSG_QUALIFIED_TYPENAME>(L, index);
		}

		OSG_QUALIFIED_TYPENAME from(lua_State* L, int index) {
			osgLua::Value * v = osgLua::Value::get(L, index);
			if (!v) {
				return OSG_QUALIFIED_TYPENAME();
			}
			return osgIntrospection::variant_cast<OSG_QUALIFIED_TYPENAME>(v->get());
		}

		void to(lua_State* L, OSG_QUALIFIED_TYPENAME const& x) {
			osgLua::Value::push(L, x);
		}
	};

}


//-- Reference Types --//
namespace luabind {

	template <typename T>
	struct default_converter< T*,
		typename boost::enable_if<
			typename boost::is_base_of< ::osg::Object, T>::type
		>::type >
	: osglua_ref_converter_base<T>
	{};

	template <typename T>
	struct default_converter< ::osg::ref_ptr<T> >
	: osglua_ref_converter_base<T, ::osg::ref_ptr<T> >
	{};

	namespace detail {
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

		template <typename T>
		struct type_to_string<T,
			typename boost::enable_if<
				typename boost::is_base_of< ::osg::Object, T>::type
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
	struct OSGTraitTrue {
		static const bool value = true;
		static const bool truevalue = true;
		typedef void type;
	};

	template <typename T>
	struct IsOSGValue {
		static const bool value = false;
	};

	template <typename T>
	struct default_converter< T,
		typename boost::enable_if_c<
			IsOSGValue<T>::value
		>::type
	>
	: osglua_val_converter_base<T>
	{};
	namespace detail {

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
	// Anything defined as an OSG Value does not inherit from osg::Object
	BOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC2_1(typename T, is_base_and_derived, ::osg::Object, T, luabind::IsOSGValue<T>::truevalue)
}

#include <boost/type_traits/detail/bool_trait_undef.hpp>

#ifndef CREATE_OSGLUA_VALUE_CONVERTER
/// Macro to define traits to indicate to Luabind that a type is an osg value type
#define CREATE_OSGLUA_VALUE_CONVERTER(T) \
	namespace luabind { \
		template <> \
		struct IsOSGValue< T > \
		  : OSGTraitTrue \
		{}; \
		\
	}
/// Internal-use macro to forward declare and create value converter in one step
#define FORWARD_DECL_AND_CREATE_VALUE_CONVERTER(NS, T) \
	namespace NS { \
		class T; \
	} \
	CREATE_OSGLUA_VALUE_CONVERTER( NS :: T )

#endif


FORWARD_DECL_AND_CREATE_VALUE_CONVERTER(osg, Matrixd);
FORWARD_DECL_AND_CREATE_VALUE_CONVERTER(osg, Matrixf);
FORWARD_DECL_AND_CREATE_VALUE_CONVERTER(osg, Vec3d);
FORWARD_DECL_AND_CREATE_VALUE_CONVERTER(osg, Vec3f);
FORWARD_DECL_AND_CREATE_VALUE_CONVERTER(osg, Vec4d);
FORWARD_DECL_AND_CREATE_VALUE_CONVERTER(osg, Vec4f);
FORWARD_DECL_AND_CREATE_VALUE_CONVERTER(osg, Quat);

#undef FORWARD_DECL_AND_CREATE_VALUE_CONVERTER

#endif // INCLUDED_vrjugglua_osgLuaBind_h
