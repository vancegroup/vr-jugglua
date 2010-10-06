/**	@file	QTNavMain.cpp
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
#include <vrjugglua/qt-console/QTConsole.h>
#include <vrjugglua/LuaPath.h>

// Library/third-party includes
#include <vrj/Kernel/Kernel.h>


// Standard includes
// - none;

using namespace vrjLua;

int main(int argc, char * argv[]) {
	bool ret;
	//QApplication app(argc, argv);

	/// Tell it our application path
	LuaPath lp = LuaPath::instance(argv[0]);

	/// Create the script object
	LuaScript script;

	/// Create the console GUI
	QTConsole::setup(argc, argv);
	boost::shared_ptr<QTConsole> console(new QTConsole(script));
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
		console->appendToDisplay("-- Could not load osgnav-testbed module: see View->Show debug log for potential clues.");
		console->appendToDisplay("-- Application will not function properly - exit when finished viewing log.");
		/// @todo tell the console to disable any active interaction (run button, open menu item, etc)
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
