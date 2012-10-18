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
#ifndef INCLUDED_OperatorBase_h_GUID_f6f3382a_be9f_4e4c_a166_43dc8bb15d40
#define INCLUDED_OperatorBase_h_GUID_f6f3382a_be9f_4e4c_a166_43dc8bb15d40

// Internal Includes
// - none

// Library/third-party includes
// - none

// Standard includes
// - none

namespace osgTraits {
	template<typename T1, typename T2>
	struct BinaryOperatorBase {
		typedef T1 first_argument_type;
		typedef T2 second_argument_type;
	};

	template<typename T>
	struct UnaryOperatorBase {
		typedef T argument_type;
	};

	struct BinaryOperatorClassBase {
		typedef tags::BinaryOperator operator_tag;
	};

	template<typename T>
	struct first_argument_type {
		typedef typename T::first_argument_type type;
	};
	template<typename T>
	struct second_argument_type {
		typedef typename T::second_argument_type type;
	};
} // end of namespace osgTraits

#endif // INCLUDED_OperatorBase_h_GUID_f6f3382a_be9f_4e4c_a166_43dc8bb15d40
