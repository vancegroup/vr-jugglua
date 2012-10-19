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
#ifndef INCLUDED_ComputeIsMathTag_h_GUID_89b3de3f_8def_48db_8c9f_01f86fe65a21
#define INCLUDED_ComputeIsMathTag_h_GUID_89b3de3f_8def_48db_8c9f_01f86fe65a21

// Internal Includes
#include "MathTypes.h"
#include "Tags.h"

// Library/third-party includes
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/switch.hpp>

// Standard includes
// - none


namespace osgTraits {
	namespace detail {
		using boost::mpl::placeholders::_;
		using  boost::mpl::identity;
		typedef boost::mpl::list
		< boost::mpl::pair<is_math_type<_>, tags::MathType >
		, boost::mpl::pair<is_scalar<_>, tags::Scalar >
		> IsMathTagMap;

		template<typename T>
		struct ComputeIsMathTag : boost::mpl::switch_<IsMathTagMap, T> {};
	} // end of namespace detail
} // end of namespace osgTraits


#endif // INCLUDED_ComputeIsMathTag_h_GUID_89b3de3f_8def_48db_8c9f_01f86fe65a21
