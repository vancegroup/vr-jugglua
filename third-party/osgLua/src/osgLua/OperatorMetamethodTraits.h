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
#ifndef INCLUDED_OperatorMetamethodTraits_h_GUID_f50476b4_fdfa_4906_af5a_217b87918f8f
#define INCLUDED_OperatorMetamethodTraits_h_GUID_f50476b4_fdfa_4906_af5a_217b87918f8f

// Internal Includes
// - none

// Library/third-party includes
// - none

// Standard includes
// - none

namespace osgTraits {
	struct UnaryMinus;
	struct Addition;
	struct Subtraction;
	struct Multiplication;
	struct CrossProduct;
	struct Division;
}

namespace osgLua {
	template<typename Op>
	struct MetamethodName;

	template<>
	struct MetamethodName<osgTraits::UnaryMinus> {
		static const char * get() {
			return "__unm";
		}
	};

	template<>
	struct MetamethodName<osgTraits::Addition> {
		static const char * get() {
			return "__add";
		}
	};

	template<>
	struct MetamethodName<osgTraits::Subtraction> {
		static const char * get() {
			return "__sub";
		}
	};

	template<>
	struct MetamethodName<osgTraits::Multiplication> {
		static const char * get() {
			return "__mul";
		}
	};

	template<>
	struct MetamethodName<osgTraits::CrossProduct> {
		static const char * get() {
			return "__pow";
		}
	};

	template<>
	struct MetamethodName<osgTraits::Division> {
		static const char * get() {
			return "__div";
		}
	};
} // end of namespace osgLua

#endif // INCLUDED_OperatorMetamethodTraits_h_GUID_f50476b4_fdfa_4906_af5a_217b87918f8f
