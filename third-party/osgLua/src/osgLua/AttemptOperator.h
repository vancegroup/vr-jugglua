/** @file
	@brief Header

	@date 2013

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program

*/

//           Copyright Iowa State University 2013.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef INCLUDED_AttemptOperator_h_GUID_98285FDD_BE20_49A7_D04F_924CB8639F97
#define INCLUDED_AttemptOperator_h_GUID_98285FDD_BE20_49A7_D04F_924CB8639F97


// Internal Includes
// - none

// Library/third-party includes
#include <osgTraits/OperatorArity.h>

// Standard includes
// - none

namespace osgLua {
	template<typename Operator, typename T, typename Arity = typename osgTraits::get_arity<Operator>::type>
	struct AttemptOperator {
		static int attempt(lua_State * L);
	};

} // end of namespace osgLua
#endif // INCLUDED_AttemptOperator_h_GUID_98285FDD_BE20_49A7_D04F_924CB8639F97

