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
#ifndef INCLUDED_Tags_h_GUID_594c4167_70eb_4377_9401_c5d8d30dae0e
#define INCLUDED_Tags_h_GUID_594c4167_70eb_4377_9401_c5d8d30dae0e

// Internal Includes
// - none

// Library/third-party includes
// - none

// Standard includes
// - none

namespace osgTraits {
	namespace tags {
		struct Vec {
			typedef Vec type;
		};
		struct Matrix {
			typedef Matrix type;
		};
		struct Quat {
			typedef Quat type;
		};
		struct Scalar {
			typedef Scalar type;
		};

		struct MathType {
			typedef MathType type;
		};
	} // end of namespace tags
} // end of namespace osgTraits

#endif // INCLUDED_Tags_h_GUID_594c4167_70eb_4377_9401_c5d8d30dae0e
