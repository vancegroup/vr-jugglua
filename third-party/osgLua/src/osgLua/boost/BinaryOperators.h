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
#ifndef INCLUDED_BinaryOperators_h_GUID_9d5a8223_67c4_4299_99ef_30fe8607bab4
#define INCLUDED_BinaryOperators_h_GUID_9d5a8223_67c4_4299_99ef_30fe8607bab4

// Internal Includes
#include "Multiplication.h"
#include "Tags.h"
#include "IsAvailable.h"

// Library/third-party includes
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/bind.hpp>
#include <boost/mpl/transform_view.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/filter_view.hpp>
#include <boost/mpl/copy_if.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/single_view.hpp>

// Standard includes
// - none

namespace osgTraits {

	template<typename Op, typename T>
	struct OperatorBindFirst {
		//typedef typename boost::mpl::lambda< typename Op::template apply<T, boost::mpl::_1> >::type type;
		typedef boost::mpl::bind2<Op, T, boost::mpl::_1> type;
	};

	template<typename Op, typename T>
	struct OperatorBindSecond {
//		typedef typename boost::mpl::lambda< typename Op::template apply<boost::mpl::_1, T> >::type type;

		typedef boost::mpl::bind2<Op, boost::mpl::_1, T> type;
	};

	template<typename BoundOp>
	struct IsBoundAvailable {
		struct result {
			template<typename T>
			struct apply {
				typedef osgTraits::is_operator_available< typename boost::mpl::apply<BoundOp, T>::type> type;
			};
		};
		typedef result type;
	};

	template<typename BoundOp>
	struct GetAvailableOtherArgTypes {
		typedef typename boost::mpl::copy_if<osgTraits::math_types, typename IsBoundAvailable<BoundOp>::type, boost::mpl::back_inserter<boost::mpl::vector0<> > >::type type;

	};

	template<typename BoundOp>
	struct GetAvailableOpSpecializations {
		typedef typename boost::mpl::transform_view<math_and_arithmetic_types, BoundOp>::type Possibilities;
		typedef typename boost::mpl::filter_view<Possibilities, is_operator_available<boost::mpl::_1> >::type type;
	};

	typedef boost::mpl::single_view<Multiplication> BinaryOperators;

} // end of namespace osgTraits
#endif // INCLUDED_BinaryOperators_h_GUID_9d5a8223_67c4_4299_99ef_30fe8607bab4
