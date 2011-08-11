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
#ifndef INCLUDED_osgLuaBind_luabindPassingStyle_h_GUID_3e29d016_c0be_40ea_bb7b_64f3cfe8b53b
#define INCLUDED_osgLuaBind_luabindPassingStyle_h_GUID_3e29d016_c0be_40ea_bb7b_64f3cfe8b53b

// Internal Includes
#include "osgLuaBind_refPtrFwd.h"

// Library/third-party includes
#include <luabind/detail/primitives.hpp>

// Standard includes
// - none


namespace osgLuaBind {

	namespace detail {
		/// Traits template for determining the luabind passing style type based on
		/// the type of container (raw pointer or ref_ptr)
		template <typename T>
		struct luabindPassingStyle;

		template <typename T>
		struct luabindPassingStyle<T*> {
			typedef luabind::detail::by_pointer<T> type;
		};

		template <typename T>
		struct luabindPassingStyle< ::osg::ref_ptr<T> > {
			typedef luabind::detail::by_value< ::osg::ref_ptr<T> > type;
		};
	} // end of namespace detail

} // end of namespace osgLuaBind

#endif // INCLUDED_osgLuaBind_luabindPassingStyle_h_GUID_3e29d016_c0be_40ea_bb7b_64f3cfe8b53b
