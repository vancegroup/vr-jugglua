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
#ifndef INCLUDED_osgLuaBind_isSameType_h_GUID_86d2c5e3_ed4a_4ecf_9c1b_a48bef790a93
#define INCLUDED_osgLuaBind_isSameType_h_GUID_86d2c5e3_ed4a_4ecf_9c1b_a48bef790a93

// Internal Includes
// - none

// Library/third-party includes
#include <osgLua/introspection/Value>
#include <osgLua/introspection/Type>
#include <osgLua/introspection/Reflection>
#include <osgLua/introspection/Exceptions>

// Standard includes
// - none

namespace osgLuaBind {
	template <typename T>
	inline bool isSameType(osgLua::introspection::Value const& v) {
		static const osgLua::introspection::Type& destType =
		    osgLua::introspection::Reflection::getType(extended_typeid<T>());
		try {
			if (v.getType() == destType) {
				return true;
			}
		} catch (osgLua::introspection::Exception &) {
			return false;
		}
		return false;
	}


} // end of namespace osgLuaBind

#endif // INCLUDED_osgLuaBind_isSameType_h_GUID_86d2c5e3_ed4a_4ecf_9c1b_a48bef790a93
