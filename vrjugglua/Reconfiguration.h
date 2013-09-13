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

//           Copyright Iowa State University 2012.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#ifndef INCLUDED_Reconfiguration_h_GUID_D0A5B873_67E9_408F_4423_6C9DD2DC0F21
#define INCLUDED_Reconfiguration_h_GUID_D0A5B873_67E9_408F_4423_6C9DD2DC0F21

// Internal Includes
// - none

// Library/third-party includes
// - none

// Standard includes
#include <string>

namespace vrjLua {

    /// @brief Remove the entire active configuration, so that it can be
    /// replaced.
    void deconfigureAll();

    /// @brief Remove the named configuration element, if present.
    void deconfigureElementByName(std::string const &elem);

} // end of namespace vrjLua

#endif // INCLUDED_Reconfiguration_h_GUID_D0A5B873_67E9_408F_4423_6C9DD2DC0F21
