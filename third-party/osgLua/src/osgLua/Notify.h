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
#ifndef INCLUDED_Notify_h_GUID_280f6dd9_642a_4cbb_944b_e03defc97a49
#define INCLUDED_Notify_h_GUID_280f6dd9_642a_4cbb_944b_e03defc97a49

// Internal Includes
// - none

// Library/third-party includes
#include <osg/Notify>

// Standard includes
// - none


#if !defined(OSG_NOTIFY) || (defined(OSG_NOTIFY) && OSG_NOTIFY == 1)
#define OSGLUA_NOTIFY(level) if (osg::isNotifyEnabled(level)) osg::notify(level)
#endif

#ifndef OSG_INFO
#define OSG_INFO OSGLUA_NOTIFY(osg::INFO)
#endif

#endif // INCLUDED_Notify_h_GUID_280f6dd9_642a_4cbb_944b_e03defc97a49
