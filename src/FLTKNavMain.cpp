/**	@file
        @brief	implementation

        @date
        2009-2010

        @author
        Ryan Pavlik
        <rpavlik@iastate.edu> and <abiryan@ryand.net>
        http://academic.cleardefinition.com/
        Iowa State University Virtual Reality Applications Center
        Human-Computer Interaction Graduate Program
*/

//          Copyright Iowa State University 2009-2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// Internal Includes
#include <vrjugglua/fltk-console/FLTKConsole.h>
#include <vrjugglua/LuaPath.h>

// Library/third-party includes
#include <vrj/Kernel/Kernel.h>

#include <FL/Fl.H>

// Standard includes
// - none;

using namespace vrjLua;

int main(int argc, char *argv[]) {
    Fl::args(argc, argv);
    Fl::get_system_colors();

    /// Tell it our application path
    LuaPath lp = LuaPath::instance(argv[0]);

    /// Load the startup script
    LuaScript script;
    script.requireModule("osgnav-testbed");

    /// Create the console GUI
    boost::shared_ptr<FLTKConsole> console(new FLTKConsole(script));
    console->setTitle("Scenegraph Navigation Testbed");
    console->getRunBufFromLuaGlobal();

    assert(console->isValid());

    /// Run it all
    vrj::Kernel::instance()->start();
    bool ret = console->threadLoop();
    if (vrj::Kernel::instance()->isRunning()) {
        vrj::Kernel::instance()->stop();
    }

    vrj::Kernel::instance()->waitForKernelStop();
    return ret ? 0 : 1;
}
