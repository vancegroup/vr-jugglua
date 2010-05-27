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
	boost::scoped_ptr<VRApp> application(new VRApp(kernel));

	if (argc <= 1)
	{
		/// if not enough arguments.
		std::cout << std::endl << std::endl;
		std::cout << "Usage: " << argv[0] << " luascriptfile ... \n" << std::endl << std::endl;
		std::exit(1);
	}

	for (unsigned int i = 1; i < argc; ++i) {
		application->loadLuaFile(argv[i]);
	}

	kernel->start();
	kernel->setApplication(application.get());
	kernel->waitForKernelStop();

	return 0;
}
