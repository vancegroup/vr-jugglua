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
#include <luabind/object.hpp>

// Standard includes
#include <vector>
#include <string>

using namespace vrjLua;

int main(int argc, char * argv[]) {
	bool ret;

	/// Tell it our application path
	LuaPath lp = LuaPath::instance(argv[0]);

	/// Process command line args
	bool stubConsole = false;
	std::vector<std::string> files;
	std::vector<std::string> args;
	for (int i = 1; i < argc; ++i) {
		std::string arg(argv[i]);
		if (arg == "--stub") {
			stubConsole = true;
		/*
		} else if (arg.find(".jconf") != std::string::npos) {
			std::cerr << "WARNING: passing jconf files on the command line not yet supported!" << std::endl;
		*/
		} else if (arg.find(".lua") != std::string::npos) {
			std::cout << "Queuing up " << arg << " to run after startup..." << std::endl;
			files.push_back(arg);
		} else {
			//std::cerr << "Unrecognized command line argument '" << arg << "', ignoring..." << std::endl;
			args.push_back(arg);
		}
	}

	/// Create the script object
	LuaScript script;
	
	/// Create the "arg" table
	luabind::object arg = luabind::newtable(script.getLuaRawState());
	arg[0] = std::string(argv[0]);
	for (unsigned int i = 0; i < args.size(); ++i) {
		arg[i+1] = args[i];
	}
	luabind::globals(script.getLuaRawState())["arg"] = arg;

	/// Create the console GUI
	boost::shared_ptr<LuaConsole> console;
	if (stubConsole) {
		std::cout << "Creating stub console, as requested..." << std::endl;
		boost::shared_ptr<LuaConsole> temp(new StubConsole(script));
		console = temp;
	} else {
		QTConsole::setup(argc, argv);
		boost::shared_ptr<LuaConsole> temp(new QTConsole(script));
		console = temp;
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
		console->appendToDisplay("-- ERROR: Could not load osgnav-testbed module: see View->Show debug log for potential clues.");
		console->appendToDisplay("-- Application will not function properly - exit when finished viewing log.");
		/// @todo tell the console to disable any active interaction (run button, open menu item, etc)
		console->threadLoop();
#endif
		return 1;
	}


	console->getRunBufFromLuaGlobal();

	assert(console->isValid());

	for (unsigned int i = 0; i < files.size(); ++i) {
		console->addFile(files[i]);
	}

	/// Run it all
	vrj::Kernel::instance()->start();
	ret = console->threadLoop();
	if (vrj::Kernel::instance()->isRunning()) {
		vrj::Kernel::instance()->stop();
	}

	vrj::Kernel::instance()->waitForKernelStop();
	return ret ? 0 : 1;
}
