/** @file
	@brief Header forward-declaring the base of the OSG referenced object
	type hierarchy, and defining a typedef.

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
#ifndef INCLUDED_osgLuaBind_RefBase_h_GUID_001c459b_bb76_4cfd_bff8_b7033125660b
#define INCLUDED_osgLuaBind_RefBase_h_GUID_001c459b_bb76_4cfd_bff8_b7033125660b

// Internal Includes
// - none

// Library/third-party includes
// - none

// Standard includes
// - none

namespace osg {
	class Object;
} // end of namespace osg

namespace osgLuaBind {

	namespace detail {
		/// Convenience typedef for the root of the OSG referenced object class hierarchy.
		typedef ::osg::Object RefBase;
	} // end of namespace detail

} // end of namespace osgLuaBind

#endif // INCLUDED_osgLuaBind_RefBase_h_GUID_001c459b_bb76_4cfd_bff8_b7033125660b
