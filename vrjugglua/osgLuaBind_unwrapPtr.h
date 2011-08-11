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
#ifndef INCLUDED_osgLuaBind_unwrapPtr_h_GUID_f05544c4_4ac5_4ce4_85e3_5224abeb5dc3
#define INCLUDED_osgLuaBind_unwrapPtr_h_GUID_f05544c4_4ac5_4ce4_85e3_5224abeb5dc3

// Internal Includes
// - none

// Library/third-party includes
#include <osg/ref_ptr>

// Standard includes
// - none

namespace osgLuaBind {

	namespace detail {
		template <typename T>
		struct unwrapPtrBase {
			typedef T return_t;
			typedef T value_t;
			static T apply(T const& x) {
				return x;
			}
		};

		template <typename T>
		struct unwrapPtr : unwrapPtrBase<T> {};

		template <typename T>
		struct unwrapPtr<T*> : unwrapPtrBase<T*> {
			typedef T value_t;
		};

		template <typename T>
		struct unwrapPtr< ::osg::ref_ptr<T> > {
			typedef T * return_t;
			typedef T value_t;
			static T * apply(osg::ref_ptr<T> const& x) {
				return x.get();
			}
		};
	} // end of namespace detail

} // end of namespace osgLuaBind

#endif // INCLUDED_osgLuaBind_getContents_h_GUID_f05544c4_4ac5_4ce4_85e3_5224abeb5dc3
