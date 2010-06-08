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

// Standard includes
// - none;

using namespace vrjLua;

static void stopKernel() {
	vrj::Kernel::instance()->stop();
}

int main(int argc, char * argv[]) {
	vrj::Kernel::setUseCocoaWrapper(false);

	/// Load the startup script
	LuaScript script;
	script.requireModule("osgnav-testbed");

	/// Create the console GUI
	FLTKConsole console(script);
	console.setTitle("Scenegraph Navigation Testbed");
	console.setExitCallback(&stopKernel);

	/// Run it all
	console.startThread();
	console.waitForThreadStop();
	return 0;
}
