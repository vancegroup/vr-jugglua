/**	@file	FLTKNavMain.cpp
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

// Internal Includes
#include <vrjugglua/fltk-console/FLTKConsole.h>

// Library/third-party includes
#include <vrj/Kernel/Kernel.h>

#include <FL/Fl.H>

// Standard includes
// - none;

using namespace vrjLua;

FLTKConsole * g_console = NULL;

static void stopKernel() {
	vrj::Kernel::instance()->stop();
}

static void stopConsole(const int) {
	if (g_console) {
		g_console->stopThread();
	}
}

int main(int argc, char * argv[]) {
	Fl::args(argc, argv);
	Fl::get_system_colors();

#ifdef __APPLE
	vrj::Kernel::setUseCocoaWrapper(false);
#endif

#ifndef _WIN32
	/// Tell the kernel to shut down our console thread if it exits first
	vrj::Kernel::instance()->addHandlerPreCallback(stopConsole);
#endif

	/// Load the startup script
	LuaScript script;
	script.requireModule("osgnav-testbed");

	/// Create the console GUI
	boost::scoped_ptr<FLTKConsole> console(new FLTKConsole(script));
	g_console = console.get();
	console->setTitle("Scenegraph Navigation Testbed");
	console->getRunBufFromLuaGlobal();
	console->setExitCallback(&stopKernel);

	/// Put in instructions
	console->appendToDisplay("-- navtransform is your root-level group node");
	console->appendToDisplay("");
	console->appendToDisplay("-- Simulator Mode Instructions for Navigation --");
	console->appendToDisplay("-- In the simulator window:");
	console->appendToDisplay("--    Hold 'shift' and move the mouse to rotate the simulated wand");
	console->appendToDisplay("--    Click to move in the direction of the wand's tip");
	console->appendToDisplay("");

	/// Run it all
	console->startThread();
	console->waitForThreadStop();
	return 0;
}
