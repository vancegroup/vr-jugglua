/** @file
        @brief Implementation

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

// Internal Includes
#include "Reconfiguration.h"

// Library/third-party includes
#include <jccl/RTRC/ConfigManager.h>
#include <jccl/Config/Configuration.h>

// Standard includes
// - none

namespace {
    struct ActiveConfigLocker : boost::noncopyable {
        ActiveConfigLocker() { jccl::ConfigManager::instance()->lockActive(); }
        ~ActiveConfigLocker() {
            jccl::ConfigManager::instance()->unlockActive();
        }
    };
}
namespace vrjLua {

    void deconfigureAll() {
        {
            ActiveConfigLocker locker;
            jccl::ConfigManager *cm = jccl::ConfigManager::instance();
            cm->addConfigurationRemovals(cm->getActiveConfig());
        }
        unsigned num_processed(0);

        do {
            num_processed =
                jccl::ConfigManager::instance()->attemptReconfiguration();
            std::cout << "num_processed = " << num_processed << std::endl;
        } while (num_processed > 0);
    }

    void deconfigureElementByName(std::string const &elem) {
        {
            ActiveConfigLocker locker;
            jccl::ConfigManager::instance()->removeActive(elem);
        }
        unsigned num_processed(0);

        do {
            num_processed =
                jccl::ConfigManager::instance()->attemptReconfiguration();
            std::cout << "num_processed = " << num_processed << std::endl;
        } while (num_processed > 0);
    }

} // end of namespace vrjLua
