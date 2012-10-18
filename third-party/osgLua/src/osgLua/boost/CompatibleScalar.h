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
#ifndef INCLUDED_CompatibleScalar_h_GUID_d27674ff_6057_4136_9268_f854370bc94c
#define INCLUDED_CompatibleScalar_h_GUID_d27674ff_6057_4136_9268_f854370bc94c

// Internal Includes
// - none

// Library/third-party includes
#include <boost/mpl/bool.hpp>

// Standard includes
// - none

namespace osgTraits {
	template<typename T1, typename T2>
	struct CompatibleScalar {
		typedef boost::mpl::false_ type;
	};

	template<typename T>
	struct CompatibleScalar<T, T> {
		typedef boost::mpl::true_ type;
		typedef T scalar_type;
	};
	template<>
	struct CompatibleScalar<float, double> {
		typedef boost::mpl::true_ type;
		typedef double scalar_type;
	};

	template<>
	struct CompatibleScalar<double, float> {
		typedef boost::mpl::true_ type;
		typedef double scalar_type;
	};

};

#endif // INCLUDED_CompatibleScalar_h_GUID_d27674ff_6057_4136_9268_f854370bc94c
