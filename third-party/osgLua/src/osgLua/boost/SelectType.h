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
#include "MathTypes.h"
#include "OsgMathTraits.h"

// Library/third-party includes
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/deref.hpp>

// Standard includes
// - none
namespace osgTraits {
	using boost::mpl::placeholders::_;

	template<typename Category, typename Scalar, typename Dimension>
	struct SelectType {

		typedef typename boost::mpl::find_if < math_types,
		        boost::mpl::and_ <
		        boost::is_same< GetCategory<_>, Category>,
		        boost::is_same< GetScalar<_>, Scalar>,
		        boost::is_same< GetDimension<_>, Dimension>
		        > >::type iter;

		typedef typename boost::mpl::eval_if < boost::is_same<Category, tags::Scalar>,
		        boost::mpl::identity<Scalar>,
		        boost::mpl::deref<iter> >::type type;
	};
} // end of namespace osgTraits

#endif // INCLUDED_SelectType_h_GUID_342a844d_6562_4afc_a182_ec0f821fdf40
