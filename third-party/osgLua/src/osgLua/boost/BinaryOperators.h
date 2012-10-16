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

// Library/third-party includes
#include <boost/mpl/bind.hpp>

// Standard includes
// - none

namespace osgTraits {

	template<typename Op, typename T>
	struct OperatorBindFirst {
		typedef boost::mpl::bind<Op, T, boost::mpl::_1> type;
	};

	template<typename Op, typename T>
	struct OperatorBindSecond {
		typedef boost::mpl::bind<Op, boost::mpl::_1, T> type;
	};

	template<typename BoundOp>
	struct IsBoundAvailable {
		template<typename T>
		struct apply {
			typedef osgTraits::is_operator_available< typename boost::mpl::apply<BoundOp, T>::type> type;
		};
	};

	template<typename BoundOp>
	struct GetAvailableOtherArgTypes {
		typedef typename boost::mpl::copy_if<osgTraits::math_types, IsBoundAvailable<BoundOp> >::type type;
	};

	template<typename BoundOp>
	struct GetAvailableOpSpecializations {
		typedef typename boost::mpl::transform<math_and_arithmetic_types, BoundOp>::type Possibilities;
		typedef typename boost::mpl::copy_if<Possibilities, is_operator_available>::type type;
	};

	typedef boost::mpl::list<Multiplication> BinaryOperators;

} // end of namespace osgTraits
#endif // INCLUDED_BinaryOperators_h_GUID_9d5a8223_67c4_4299_99ef_30fe8607bab4
