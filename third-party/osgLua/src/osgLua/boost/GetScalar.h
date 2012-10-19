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
#ifndef INCLUDED_GetScalar_h_GUID_369a7190_525e_4eaa_ae0b_0743be34526a
#define INCLUDED_GetScalar_h_GUID_369a7190_525e_4eaa_ae0b_0743be34526a

// Internal Includes
#include "IsScalar.h"

// Library/third-party includes
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/has_xxx.hpp>

// Standard includes
// - none

namespace osgTraits {
	namespace detail {
		BOOST_MPL_HAS_XXX_TRAIT_DEF(value_type);

		template<typename T, typename = void>
		struct GetScalarImpl {};

		template<typename T>
		struct GetScalarImpl<T, typename boost::enable_if<has_value_type<T> >::type> {
			typedef typename T::value_type type;
		};

		template<typename T>
		struct GetScalarImpl<T, typename boost::enable_if<is_scalar<T> >::type> {
			typedef T type;
		};

	} // end of namespace detail

	template<typename T>
	struct GetScalar : detail::GetScalarImpl<T> {};


} // end of namespace osgTraits

#endif // INCLUDED_GetScalar_h_GUID_369a7190_525e_4eaa_ae0b_0743be34526a
