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
#include <vpr/Thread/Thread.h>

#include <boost/bind.hpp>

#include <FL/Fl.H>

// Standard includes
// - none;

using namespace vrjLua;

static void stopKernel() {
	vrj::Kernel::instance()->stop();
}

int main(int argc, char * argv[]) {
	Fl::args(argc, argv);
	Fl::get_system_colors();

	/// Load the startup script
	LuaScript script;
	script.requireModule("osgnav-testbed");

	/// Create the console GUI
	boost::shared_ptr<FLTKConsole> console(new FLTKConsole(script));
	console->setTitle("Scenegraph Navigation Testbed");
	console->getRunBufFromLuaGlobal();

	assert(console->isValid());

	/// Put in instructions
	console->appendToDisplay("-- navtransform is your root-level group node");
	console->appendToDisplay("");
	console->appendToDisplay("-- Simulator Mode Instructions for Navigation --");
	console->appendToDisplay("-- In the simulator window:");
	console->appendToDisplay("--    Hold 'shift' and move the mouse to rotate the simulated wand");
	console->appendToDisplay("--    Click to move in the direction of the wand's tip");
	console->appendToDisplay("");

	/// Run it all
	vpr::Thread thread(boost::bind(&FLTKConsole::threadLoop, console.get()));
	vrj::Kernel::instance()->start();
	vrj::Kernel::instance()->waitForKernelStop();
	thread.join();
	return 0;
}
