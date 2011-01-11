/**	@file	binding_detail/BindKernelToLua.inl
	@brief	implementation of kernel and print overload binding

	@date
	2009-2010

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

// Local includes
#ifndef LUABIND_COMBINED_COMPILE
#include <vrjugglua/osgLuaBind.h>
#endif
#include "BindKernelToLua.h"
#include <vrjugglua/VRJLuaOutput.h>

// Library/third-party includes
#include <luabind/luabind.hpp>

#include <vrj/Kernel/Kernel.h>
#include <vpr/Thread/Thread.h>
#include <vpr/System.h>

#if __VJ_version < 2003011
#	include <vrj/Draw/OSG/OsgApp.h>
	typedef vrj::OsgApp OsgApp;
#else
#	include <vrj/Draw/OSG/App.h>
	typedef vrj::osg::App OsgApp;
#endif

// Standard includes
#ifdef VERBOSE
#include <iostream>
#endif

namespace vrjLua {
using namespace luabind;

// Static member
bool KernelState::_init = false;


bool KernelState::hasInitialized() {
	return _init;
}

#if __VJ_version >= 2003000
// Juggler 3.x - initialize the kernel
boost::program_options::options_description KernelState::getVrjOptionsDescriptions() {
	boost::program_options::options_description desc("VR Juggler 3.x Options");
	desc.add(vrj::Kernel::instance()->getClusterOptions()).add(vrj::Kernel::instance()->getConfigOptions());
	return desc;
}

void KernelState::init(boost::program_options::variables_map vm) {
	if (_init) {
		VRJLUA_MSG_START(dbgVRJLUA, MSG_WARNING)
		<< "Warning: vrjKernel.init called a second time!"
		<< VRJLUA_MSG_END(dbgVRJLUA, MSG_WARNING);
	}
	vrj::Kernel::instance()->init(vm);
	_init = true;
}

void KernelState::init(int argc, char* argv[]) {
	if (_init) {
		VRJLUA_MSG_START(dbgVRJLUA, MSG_WARNING)
		<< "Warning: vrjKernel.init called a second time!"
		<< VRJLUA_MSG_END(dbgVRJLUA, MSG_WARNING);
	}
	vrj::Kernel::instance()->init(argc, argv);
	_init = true;
}

/// Fallback - if we have no other source of info
void KernelState::init() {
	if (_init) {
		VRJLUA_MSG_START(dbgVRJLUA, MSG_WARNING)
		<< "Warning: vrjKernel.init called a second time!"
		<< VRJLUA_MSG_END(dbgVRJLUA, MSG_WARNING);
	}

	VRJLUA_MSG_START(dbgVRJLUA, MSG_WARNING)
	<< "Warning: your application is not cluster-capable as compiled (VR Juggler 3.x) because you did not initialize the kernel with command line arguments!"
	<< VRJLUA_MSG_END(dbgVRJLUA, MSG_WARNING);
	boost::program_options::variables_map vm;
	vrj::Kernel::instance()->init(vm);
	_init = true;
}

#else
// Juggler 2.2 - stub methods
boost::program_options::options_description KernelState::getVrjOptionsDescriptions() {
	return boost::program_options::options_description();
}

void KernelState::init(boost::program_options::variables_map /*vm*/) {
	// no-op
	_init = true;
}

void KernelState::init(int /*argc*/, char* /*argv*/[]) {
	// no-op
	_init = true;
}

void KernelState::init() {
	// no-op
	VRJLUA_MSG_START(dbgVRJLUA, MSG_WARNING)
	<< "Warning: your application will not cluster-capable with VR Juggler 3.x because you did not initialize the kernel with command line arguments!"
	<< VRJLUA_MSG_END(dbgVRJLUA, MSG_WARNING);

	_init = true;
}
#endif

namespace Kernel {
	
	void start(bool waiting = false) {
		if (!KernelState::hasInitialized()) {
			VRJLUA_MSG_START(dbgVRJLUA, MSG_WARNING)
			<< "Warning: vrjKernel.start called before vrjKernel.init - your app will not support clustering in VR Juggler 3.x!"
			<< VRJLUA_MSG_END(dbgVRJLUA, MSG_WARNING);
			KernelState::init();
		}
		
		if (!waiting) {
			VRJLUA_MSG_START(dbgVRJLUA, MSG_WARNING)
				<< "Warning: vrjKernel.start called: possible race condition if you run Lua code in the frame loop! (Use vrjKernel.enter to avoid this)"
				<< VRJLUA_MSG_END(dbgVRJLUA, MSG_WARNING);
		}
		vrj::Kernel::instance()->start();
	}

	void stop() {
		if (vpr::Thread::self() != vrj::Kernel::instance()->getThread()) {
			/// We are not the kernel control thread - do a sleep here
			/// to reduce race condition issues with quick startup/shutdown (like in tests)

			VRJLUA_MSG_START(dbgVRJLUA, MSG_STATUS)
				<< "vrjKernel.stop() called from outside the control thread - sleeping for 2 sec for thread safety"
				<< VRJLUA_MSG_END(dbgVRJLUA, MSG_STATUS);
			vpr::Thread::yield();
			vpr::System::sleep(2);
		}
		vrj::Kernel::instance()->stop();
	}

	void waitForKernelStop() {
		vrj::Kernel::instance()->waitForKernelStop();
	}
	
	void enter() {
		start(true);
		waitForKernelStop();	
	}

	void setApplication(luabind::object app) {
		vrj::Kernel::instance()->setApplication(luabind::object_cast<OsgApp*>(app));
	}

	void loadConfigFile(const std::string & fn) {
		vrj::Kernel::instance()->loadConfigFile(fn);
	}

	bool isRunning() {
		return vrj::Kernel::instance()->isRunning();
	}

} // end of Kernel namespace

void bindKernelToLua(LuaStatePtr state) {
#ifdef VERBOSE
	std::cerr << "Registering vrjKernel module functions with Lua..." << std::flush << std::endl;
#endif
	module(state.get(), "vrjKernel") [
		def("enter", &Kernel::enter),
		def("start", &Kernel::start),
		def("stop", &Kernel::stop),
		def("setApplication", &Kernel::setApplication),
		def("loadConfigFile", &Kernel::loadConfigFile),
		def("waitForKernelStop", &Kernel::waitForKernelStop),
		def("isRunning", &Kernel::isRunning),
		def("safePrint", &LuaScript::doPrint) /// @todo move this into a more generic/shared namesapce
	];

	// Hide the print function, using the threadsafe one.
	luaL_dostring(state.get(),
	"do \n\
		local oldprint = print \n\
		print = function(s) \n\
			vrjKernel.safePrint(tostring(s)) \n\
		end \n\
	end");

}

}// end of vrjLua namespace
