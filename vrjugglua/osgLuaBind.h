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

#include <osgLua/Value.h>
#include <osgIntrospection/variant_cast>

// Standard includes
#ifdef BUILD_VERBOSE
#include <iostream>
#endif

namespace luabind
{
	/// Base class for converting osg referenced types to/from osgLua values
	template<typename OSG_QUALIFIED_TYPENAME>
	struct osglua_ref_converter_base : native_converter_base<OSG_QUALIFIED_TYPENAME*> {
		static int compute_score(lua_State* L, int index) {
			osgLua::Value * v = osgLua::Value::get(L, index);
			if (!v) {
#ifdef BUILD_VERBOSE
				std::cout << "Not a osgLua value" << std::endl;
#endif
				return -1;
			}
			static const osgIntrospection::Type& destType =
				osgIntrospection::Reflection::getType(extended_typeid<OSG_QUALIFIED_TYPENAME*>());
			const osgIntrospection::Type& type = v->get().getType();
#ifdef BUILD_VERBOSE
			std::cout << "Destination type: " << destType.getQualifiedName() << std::endl;
			std::cout << "Value type: " << v->get().getType().getQualifiedName() << std::endl;
#endif
			try {
				if (type == destType) {
#ifdef BUILD_VERBOSE
					std::cout << "Exact match for type!" << std::endl;
#endif
					return 2;
				} else if (osgIntrospection::variant_cast<OSG_QUALIFIED_TYPENAME*>(v->get()) != NULL) {
					if (osgIntrospection::requires_conversion<OSG_QUALIFIED_TYPENAME*>(v->get())) {
#ifdef BUILD_VERBOSE
						std::cout << "Convertible match for type." << std::endl;
#endif
						return 0;
					} else {
#ifdef BUILD_VERBOSE
						std::cout << "Polymorphic match for type." << std::endl;
#endif
						return 1;
					}
				}
			} catch (...) {
				/// @todo make this catch only osgIntrospection exceptions
				return -1;
			}
			return -1;
		}

		int match(lua_State* L, detail::by_pointer<OSG_QUALIFIED_TYPENAME>, int index) {
			return compute_score(L, index);
		}

		OSG_QUALIFIED_TYPENAME* from(lua_State* L, int index) {
			osgLua::Value * v = osgLua::Value::get(L, index);
			if (!v) {
				return NULL;
			}
			return osgIntrospection::variant_cast<OSG_QUALIFIED_TYPENAME*>(v->get());
		}

		OSG_QUALIFIED_TYPENAME* apply(lua_State* L, detail::by_pointer<OSG_QUALIFIED_TYPENAME>, int index) {
			return from(L, index);
		}
		
		void apply(lua_State* L, OSG_QUALIFIED_TYPENAME* const& value) {
			to(L, value);
		}

		void to(lua_State* L, OSG_QUALIFIED_TYPENAME* const& x) {
			osgLua::Value::push(L, x);
		}
	};
	
	template<typename OSG_QUALIFIED_TYPENAME>
	struct osglua_ref_refptr_converter_base : native_converter_base< osg::ref_ptr<OSG_QUALIFIED_TYPENAME> > {
		static int compute_score(lua_State* L, int index) {
			osgLua::Value * v = osgLua::Value::get(L, index);
			if (!v) {
#ifdef BUILD_VERBOSE
				std::cout << "Not a osgLua value" << std::endl;
#endif
				return -1;
			}
			static const osgIntrospection::Type& destType =
				osgIntrospection::Reflection::getType(extended_typeid<OSG_QUALIFIED_TYPENAME*>());
			const osgIntrospection::Type& type = v->get().getType();
#ifdef BUILD_VERBOSE
			std::cout << "Destination type: " << destType.getQualifiedName() << std::endl;
			std::cout << "Value type: " << v->get().getType().getQualifiedName() << std::endl;
#endif
			try {
				if (type == destType) {
#ifdef BUILD_VERBOSE
					std::cout << "Exact match for type!" << std::endl;
#endif
					return 2;
				} else if (osgIntrospection::variant_cast<OSG_QUALIFIED_TYPENAME*>(v->get()) != NULL) {
					if (osgIntrospection::requires_conversion<OSG_QUALIFIED_TYPENAME*>(v->get())) {
#ifdef BUILD_VERBOSE
						std::cout << "Convertible match for type." << std::endl;
#endif
						return 0;
					} else {
#ifdef BUILD_VERBOSE
						std::cout << "Polymorphic match for type." << std::endl;
#endif
						return 1;
					}
				}
			} catch (...) {
				/// @todo make this catch only osgIntrospection exceptions
				return -1;
			}
			return -1;
		}

		int match(lua_State* L, detail::by_pointer<OSG_QUALIFIED_TYPENAME>, int index) {
			return compute_score(L, index);
		}
		
		int match(lua_State* L, detail::by_value<osg::ref_ptr<OSG_QUALIFIED_TYPENAME> >&, int index) {
			return compute_score(L, index);
		}

		osg::ref_ptr<OSG_QUALIFIED_TYPENAME> from(lua_State* L, int index) {
			osgLua::Value * v = osgLua::Value::get(L, index);
			if (!v) {
				return osg::ref_ptr<OSG_QUALIFIED_TYPENAME>();
			}
			return osg::ref_ptr<OSG_QUALIFIED_TYPENAME>(osgIntrospection::variant_cast<OSG_QUALIFIED_TYPENAME*>(v->get()));
		}

		osg::ref_ptr<OSG_QUALIFIED_TYPENAME> apply(lua_State* L, detail::by_pointer<OSG_QUALIFIED_TYPENAME>, int index) {
			return from(L, index);
		}
		
		osg::ref_ptr<OSG_QUALIFIED_TYPENAME> apply(lua_State* L, detail::by_value<osg::ref_ptr<OSG_QUALIFIED_TYPENAME> >&, int index) {
			return from(L, index);
		}
		
		void apply(lua_State* L, osg::ref_ptr<OSG_QUALIFIED_TYPENAME> const& value) {
			to(L, value);
		}

		void to(lua_State* L, osg::ref_ptr<OSG_QUALIFIED_TYPENAME> const& x) {
			osgLua::Value::push(L, x.get());
		}
	};

	/// Base class for converting osg value types to/from osgLua values
	template<typename OSG_QUALIFIED_TYPENAME>
	struct osglua_val_converter_base : native_converter_base<OSG_QUALIFIED_TYPENAME> {
		static int compute_score(lua_State* L, int index) {
			osgLua::Value * v = osgLua::Value::get(L, index);
			if (!v) {
#ifdef BUILD_VERBOSE
				std::cout << "Not a osgLua value" << std::endl;
#endif
				return -1;
			}
			static const osgIntrospection::Type& destType =
				osgIntrospection::Reflection::getType(extended_typeid<OSG_QUALIFIED_TYPENAME>());
			const osgIntrospection::Type& type = v->get().getType();

#ifdef BUILD_VERBOSE
			std::cout << "Destination type: " << destType.getQualifiedName() << std::endl;
			std::cout << "Value type: " << type.getQualifiedName() << std::endl;
#endif

			if (type == destType) {
#ifdef BUILD_VERBOSE
				std::cout << "Exact match for type!" << std::endl;
#endif
				return 2;
			} else if (type.isSubclassOf(destType)) {
#ifdef BUILD_VERBOSE
				std::cout << "Convertible match for type." << std::endl;
#endif
				return 1;
			} else {
				// attempting conversion
				try {
					OSG_QUALIFIED_TYPENAME temp = osgIntrospection::variant_cast<OSG_QUALIFIED_TYPENAME>(v->get());
				} catch (...) {
					return -1;
				}
				
				return 0; // OK, convertible, so better than nothing
			}
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

#ifndef CREATE_OSGLUA_REFERENCE_CONVERTER
/// Macro to create converters required to bind functions with osg-typed
/// referenced arguments with Luabind
#define CREATE_OSGLUA_REFERENCE_CONVERTER(T) \
	namespace luabind { \
		template <> \
		struct default_converter<T*> \
		  : osglua_ref_converter_base<T> \
		{}; \
		\
		template <> \
		struct default_converter<T* const&> \
		  : default_converter<T*> \
		{}; \
		\
		template <> \
		struct default_converter< osg::ref_ptr<T> > \
		  : osglua_ref_refptr_converter_base<T> \
		{}; \
		\
		namespace detail {\
			template <> \
			struct type_to_string<T*> \
			{ \
				static void get(lua_State* L) \
				{ \
				    lua_pushstring(L, #T " *"); \
				} \
			}; \
			\
			template <> \
			struct type_to_string< osg::ref_ptr<T> > \
			{ \
				static void get(lua_State* L) \
				{ \
				    lua_pushstring(L, "osg::ref_ptr<" #T ">"); \
				} \
			}; \
		}\
    }
#endif

#include <osg/Node>
#include <osg/Group>
CREATE_OSGLUA_REFERENCE_CONVERTER(osg::Node);
CREATE_OSGLUA_REFERENCE_CONVERTER(osg::Group);

//-- Value Types --//

#ifndef CREATE_OSGLUA_VALUE_CONVERTER
/// Macro to create converters required to bind functions with osg-typed
/// value arguments with Luabind
#define CREATE_OSGLUA_VALUE_CONVERTER(T) \
	namespace luabind { \
		template <> \
		struct default_converter<T> \
		  : osglua_val_converter_base<T> \
		{}; \
		\
		template <> \
		struct default_converter<T const&> \
		  : default_converter<T> \
		{}; \
		\
		namespace detail {\
			template <> \
			struct type_to_string<T> \
			{ \
				static void get(lua_State* L) \
				{ \
				    lua_pushstring(L, #T); \
				} \
			}; \
		}\
    }
#endif

#include <osg/Matrixd>
#include <osg/Vec3d>
#include <osg/Vec4d>
#include <osg/Quat>

CREATE_OSGLUA_VALUE_CONVERTER(osg::Matrixd);
CREATE_OSGLUA_VALUE_CONVERTER(osg::Vec3d);
CREATE_OSGLUA_VALUE_CONVERTER(osg::Vec4d);
CREATE_OSGLUA_VALUE_CONVERTER(osg::Quat);

#endif // INCLUDED_vrjugglua_osgLuaBind_h
