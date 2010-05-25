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
#include <vrjugglua/LuaScript.h>

// Library includes
#include <boost/scoped_ptr.hpp>
#include <vrj/Kernel/Kernel.h>

// Standard includes
#include <cstdlib>
#include <iostream>
#include <string>

/**********************************************************\
 main() -  gets config files.
\**********************************************************/
int main(int argc, char* argv[])
{
	vrj::Kernel* kernel = vrj::Kernel::instance();

	if (argc <= 2)
	{
		/// if not enough arguments.
		std::cout << std::endl << std::endl;
		std::cout << "Usage: " << argv[0] << " luascriptfile vjconfigfile[0] vjconfigfile[1] ... vjconfigfile[n]\n" << std::endl << std::endl;
		std::exit(1);
	}

	std::string luaFn = argv[1];

	/// Load any config files specified on the command line
	for (int i = 2; i < argc; ++i) {
		kernel->loadConfigFile(argv[i]);
		std::cout << "VRJuggler config file is " << argv[i] << std::endl;
	}

	std::cout << "------------------------" << std::endl;
	std::cout << "Configuration complete, starting kernel..." << std::endl;
	std::cout << "------------------------" << std::endl;

	kernel->start();
	vrjLua::LuaScript _script;
	_script.runFile(luaFn);
	kernel->waitForKernelStop();

	return 0;
}
