/** @file
	@brief Header

	@date 2012

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

//          Copyright Iowa State University 2012.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#ifndef INCLUDED_RegisterMathMetamethods_h_GUID_2338d460_fd97_40ce_bff4_068da65d08bd
#define INCLUDED_RegisterMathMetamethods_h_GUID_2338d460_fd97_40ce_bff4_068da65d08bd

// Internal Includes
#include "Value_metamethods.h"
#include <osgLua/LuaInclude>
#include <osgLua/introspection/Type>

// Library/third-party includes
// - none

// Standard includes
// - none

namespace osgLua {
	/// @brief Pass a Lua state with a new metatable on top, and an
	/// introspection::Type to register any math methods and return info
	/// on comparability.
	Comparability registerMathMetamethods(lua_State * L, introspection::Type const& t);

} // end of namespace osgLua

#endif // INCLUDED_RegisterMathMetamethods_h_GUID_2338d460_fd97_40ce_bff4_068da65d08bd
