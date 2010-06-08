/**	@file	NavInteractive.cpp
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
//#include "FLTKNav.h"
#include <vrjugglua/fltk-console/FLTKConsole.h>

// Library/third-party includes
#include <vrj/Kernel/Kernel.h>
#include <boost/bind.hpp>

// Standard includes
#include <iostream>

using namespace vrjLua;

static void stopKernel() {
	vrj::Kernel::instance()->stop();
}

int main(int argc, char * argv[]) {
	vrj::Kernel::setUseCocoaWrapper(false);
	//FLTKNav nav;
	LuaScript script;
	script.requireModule("osgnav-testbed");
	FLTKConsole console(script);
	console.setExitCallback(&stopKernel);
	console.startThread();
	console.waitForThreadStop();
	return 0;
}
