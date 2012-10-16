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
#ifndef INCLUDED_SelectType_h_GUID_342a844d_6562_4afc_a182_ec0f821fdf40
#define INCLUDED_SelectType_h_GUID_342a844d_6562_4afc_a182_ec0f821fdf40

// Internal Includes
#include "OsgMathTraits.h"

// Library/third-party includes
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/equal.hpp>

// Standard includes
// - none
namespace osgTraits {
	template<typename Detail>
	struct SelectType {

		typedef typename boost::mpl::find_if<math_types, boost::mpl::equal<GetMathTypeDetail<boost::mpl::_1>, Detail> >::type iter;
		typedef typename boost::mpl::eval_if < boost::mpl::equal<typename Detail::category_tag, tags::Scalar>,
		        boost::mpl::identity<typename Detail::scalar_tag>,
		        boost::mpl::deref<iter> >::type type;
	};
} // end of namespace osgTraits

#endif // INCLUDED_SelectType_h_GUID_342a844d_6562_4afc_a182_ec0f821fdf40
