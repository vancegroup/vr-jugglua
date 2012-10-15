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

// Standard includes
// - none
namespace osgTraits {
	template<typename Detail>
	struct SelectType {

		/*
		typedef typename Detail::category_tag category;
		typedef typename Detail::scalar_tag scalar;
		typedef typename Detail::dimension_tag dimension;
		typedef boost::mpl::and_<
			boost::is_same<GetCategory<_1>, category>,
			boost::is_same<GetDimension<_1>, dimension>,
			boost::is_same<GetScalar<_1>, scalar> > */

		typedef typename find_if<math_types, is_same<GetMathTypeDetail<_1>, Detail> >::type iter;
		typedef typename boost::eval_if < boost::is_same<Detail::category_tag, tags::Scalar>,
		        boost::mpl::identity<Detail::scalar_tag>,
		        deref<iter> >::type type;
	};
} // end of namespace osgTraits

#endif // INCLUDED_SelectType_h_GUID_342a844d_6562_4afc_a182_ec0f821fdf40
