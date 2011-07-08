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
#ifndef INCLUDED_osgLuaBind_isValueType_h_GUID_9fe524ca_d43a_4ae1_8abe_b29b53f05918
#define INCLUDED_osgLuaBind_isValueType_h_GUID_9fe524ca_d43a_4ae1_8abe_b29b53f05918

// Internal Includes
#include "osgLuaBind_traitBase.h"

// Library/third-party includes
// - none

// Standard includes
// - none

namespace osgLuaBind {

	/// Templated type trait: all types are assumed to
	/// not be OSG value types unless otherwise stated.
	template <typename T>
	struct IsValueType : TraitFalse
	{};

} // end of namespace osgLuaBind

#ifndef OSGLUABIND_IS_VALUE_TYPE

/// Macro to define trait to indicate to Luabind that a type is an osg value type.
/// If you're binding a function/method/property that involves an OSG value type
/// not already mentioned at the bottom of this header, you'll need to use this
/// macro in your source file to tag the type you're using as an OSG value type.
/// Failing to do so will give you a weird error at runtime when trying to use that
/// bound item that reads something like "No matching overload found, candidates:
/// void functionname(custom [Z7osg...])"
#define OSGLUABIND_IS_VALUE_TYPE(T) \
	namespace osgLuaBind { \
		template <> \
		struct IsValueType< T > \
		  : TraitTrue \
		{}; \
		\
	} /* end of namespace osgLuaBind */

/// Internal-use macro to forward declare and create value type trait in one step.
#define FORWARD_DECL_AND_DEFINE_VALUE_TRAIT(NS, T) \
	namespace NS { \
		class T; \
	} \
	OSGLUABIND_IS_VALUE_TYPE( NS :: T )

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

FORWARD_DECL_AND_DEFINE_VALUE_TRAIT(osg, Matrixd);
FORWARD_DECL_AND_DEFINE_VALUE_TRAIT(osg, Matrixf);
FORWARD_DECL_AND_DEFINE_VALUE_TRAIT(osg, Vec3d);
FORWARD_DECL_AND_DEFINE_VALUE_TRAIT(osg, Vec3f);
FORWARD_DECL_AND_DEFINE_VALUE_TRAIT(osg, Vec4d);
FORWARD_DECL_AND_DEFINE_VALUE_TRAIT(osg, Vec4f);
FORWARD_DECL_AND_DEFINE_VALUE_TRAIT(osg, Quat);

/// @}

// Sorry, you probably shouldn't use this macro in your own files.
#undef FORWARD_DECL_AND_DEFINE_VALUE_TRAIT


#endif // ifndef OSGLUABIND_IS_VALUE_TYPE

#endif // INCLUDED_osgLuaBind_isValueType_h_GUID_9fe524ca_d43a_4ae1_8abe_b29b53f05918
