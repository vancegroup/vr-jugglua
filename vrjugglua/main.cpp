/**	@file	main.cpp
	@brief	Lua VR Juggler application launcher

	@date
	2009-2010

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

// Internal includes
#include "LuaOSGApp.h"

// Library includes
#include <boost/scoped_ptr.hpp>

// Standard includes
#include <cstdlib>
#include <iostream>

/**********************************************************\
 main() -  gets config files.
\**********************************************************/
int main(int argc, char* argv[])
{
	vrj::Kernel* kernel = vrj::Kernel::instance();
	boost::scoped_ptr<VRApp> application(new VRApp(kernel, argc, argv));

	if (argc <= 2)
	{
		/// if not enough arguments.
		std::cout << std::endl << std::endl;
		std::cout << "Usage: " << argv[0] << " luascriptfile vjconfigfile[0] vjconfigfile[1] ... vjconfigfile[n]\n" << std::endl << std::endl;
		std::exit(1);
	}

	application->loadLuaFile(argv[1]);

	/// Load any config files specified on the command line
	for (int i = 2; i < argc; ++i) {
		kernel->loadConfigFile(argv[i]);
		std::cout << "VRJuggler config file is " << argv[i] << std::endl;
	}

	std::cout << "------------------------" << std::endl;
	std::cout << "Configuration complete, starting kernel..." << std::endl;
	std::cout << "------------------------" << std::endl;

	kernel->start();
	kernel->setApplication(application.get());
	kernel->waitForKernelStop();

	return 0;
}
