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
#ifndef INCLUDED_ComputeCategoryTag_h_GUID_49745613_01fb_41d6_a1a0_7a8e3fc30098
#define INCLUDED_ComputeCategoryTag_h_GUID_49745613_01fb_41d6_a1a0_7a8e3fc30098

// Internal Includes
#include "MathTypes.h"
#include "IsScalar.h"
#include "Tags.h"

// Library/third-party includes
#include <boost/utility/enable_if.hpp>

#include <boost/mpl/quote.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/switch.hpp>

// Standard includes
// - none

#undef USE_SWITCH_INSTEAD_OF_SFINAE
namespace osgTraits {
	namespace detail {
#ifdef USE_SWITCH_INSTEAD_OF_SFINAE
		typedef boost::mpl::list
		< boost::mpl::pair<boost::mpl::quote1<is_matrix>, tags::Matrix>
		, boost::mpl::pair<boost::mpl::quote1<is_vector>, tags::Vec>
		, boost::mpl::pair<boost::mpl::quote1<is_quat>, tags::Quat>
		, boost::mpl::pair<boost::mpl::quote1<is_scalar>, tags::Scalar> > CategoryTagMap;

		template<typename T>
		struct ComputeCategoryTag : boost::mpl::switch_<CategoryTagMap, T> {};
#else
		template<typename T, typename = void>
		struct ComputeCategoryTag {
			typedef void type;
		};

		template<typename T>
		struct ComputeCategoryTag<T, typename boost::enable_if<is_matrix<T> >::type> {
			typedef tags::Matrix type;
		};

		template<typename T>
		struct ComputeCategoryTag<T, typename boost::enable_if<is_vector<T> >::type> {
			typedef tags::Vec type;
		};

		template<typename T>
		struct ComputeCategoryTag<T, typename boost::enable_if<is_quat<T> >::type> {
			typedef tags::Quat type;
		};

		template<typename T>
		struct ComputeCategoryTag<T, typename boost::enable_if<is_scalar<T> >::type> {
			typedef tags::Scalar type;
		};
#endif

	} // end of namespace detail
} // end of namespace osgTraits

#endif // INCLUDED_ComputeCategoryTag_h_GUID_49745613_01fb_41d6_a1a0_7a8e3fc30098
