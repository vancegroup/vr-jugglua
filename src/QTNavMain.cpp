/**	@file
        @brief	implementation

        @date
        2009-2011

        @author
        Ryan Pavlik
        <rpavlik@iastate.edu> and <abiryan@ryand.net>
        http://academic.cleardefinition.com/
        Iowa State University Virtual Reality Applications Center
        Human-Computer Interaction Graduate Program
*/

//          Copyright Iowa State University 2009-2011.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// Internal Includes
#include <vrjugglua/qt-console/QTConsole.h>
#include <vrjugglua/LuaConsole.h>
#include <vrjugglua/LuaScript.h>
#include <vrjugglua/LuaPath.h>

// Library/third-party includes
#include <vrj/Kernel/Kernel.h>
#include <luabind/object.hpp>
#include <boost/make_shared.hpp>

// Standard includes
#include <vector>
#include <string>
#include <cassert>
#include <iostream>

using namespace vrjLua;

int main(int argc, char *argv[]) {
    bool ret;

    /// Tell it our application path
    LuaPath lp = LuaPath::instance(argv[0]);

    /// Process command line args
    bool stubConsole = false;
    std::vector<std::string> args;
    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        if (arg == "--stub") {
            stubConsole = true;
        } else {
            args.push_back(arg);
        }
    }

    /// Create the script object
    LuaScript script;

    /// Create the "arg" table
    luabind::object arg = luabind::newtable(script.getLuaRawState());
    arg[0] = std::string(argv[0]);
    for (unsigned int i = 0; i < args.size(); ++i) {
        arg[i + 1] = args[i];
    }
    luabind::globals(script.getLuaRawState())["arg"] = arg;

    /// Create the console GUI
    boost::shared_ptr<LuaConsole> console;
    if (stubConsole) {
        std::cout << "Creating stub console, as requested..." << std::endl;
        console = boost::make_shared<StubConsole>(script);
    } else {
        QTConsole::setup(argc, argv);
        console = boost::make_shared<QTConsole>(script);
    }
    console->setTitle("Scenegraph Navigation Testbed");
#ifdef BUILD_WITHOUT_TERMINAL
    console->captureStdOut();
    console->captureStdErr();
#endif

    /// Load the startup script
    ret = script.requireModule("osgnav-testbed");
    if (!ret) {
        std::cout << "Could not load osgnav-testbed module!" << std::endl;
#ifdef BUILD_WITHOUT_TERMINAL
        console->appendToDisplay("-- ERROR: Could not load osgnav-testbed "
                                 "module: see View->Show debug log for "
                                 "potential clues.");
        console->appendToDisplay("-- Application will not function properly - "
                                 "exit when finished viewing log.");
        console->disableAction();
        console->threadLoop();
#endif
        return 1;
    }

    console->getRunBufFromLuaGlobal();

    assert(console->isValid());

    /// Run it all
    vrj::Kernel::instance()->start();
    ret = console->threadLoop();
    if (vrj::Kernel::instance()->isRunning()) {
        vrj::Kernel::instance()->stop();
    }

    vrj::Kernel::instance()->waitForKernelStop();
    return ret ? 0 : 1;
}
