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

//          Copyright Iowa State University 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef INCLUDED_PathSetupDummy_h_GUID_df86b7f4_4c15_4b24_8ba7_7f980a0013ff
#define INCLUDED_PathSetupDummy_h_GUID_df86b7f4_4c15_4b24_8ba7_7f980a0013ff

// Internal Includes
#include <vrjugglua/LuaScript.h>

// Library/third-party includes
// - none

// Standard includes
#include <iostream>

struct InitPath {
    InitPath() {
        std::cerr << "Initializing test suite! "
                  << boost::unit_test::framework::master_test_suite().argv[0]
                  << std::endl;
        vrjLua::LuaScript::initWithArgv0(
            boost::unit_test::framework::master_test_suite().argv[0]);
    }
};

#endif // INCLUDED_PathSetupDummy_h_GUID_df86b7f4_4c15_4b24_8ba7_7f980a0013ff
