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

	/// Load the startup script
	LuaScript script;
	ret = script.requireModule("osgnav-testbed");
	if (!ret) {
		std::cout << "Could not load osgnav-testbed module!" << std::endl;
		return 1;
	}

	/// Create the console GUI
	QTConsole::setup(argc, argv);
	boost::shared_ptr<QTConsole> console(new QTConsole(script));
	console->setTitle("Scenegraph Navigation Testbed");
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
