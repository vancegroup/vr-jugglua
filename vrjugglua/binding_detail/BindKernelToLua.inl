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

//          Copyright Iowa State University 2010-2011.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


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
	typedef vrj::OsgApp KernelOsgApp;
#else
#	include <vrj/Draw/OSG/App.h>
	typedef vrj::osg::App KernelOsgApp;
#endif

// Standard includes
#ifdef VERBOSE
#include <iostream>
#endif

namespace vrjLua {
using namespace luabind;

// Static members
bool KernelState::_init = false;
bool KernelState::_isCluster = false;
bool KernelState::_isPrimary = false;


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
	bool isPrimary = vm.count("vrjmaster") && vm["vrjmaster"].as<bool>();
	bool isSecondary = vm.count("vrjslave") && vm["vrjslave"].as<bool>();
	if (isPrimary || isSecondary) {
		_isCluster = true;
	}
	_isPrimary = isPrimary;
	
	vrj::Kernel::instance()->init(vm);
	_init = true;
}

void KernelState::init(int argc, char* argv[]) {
	boost::program_options::options_description& cluster_desc = vrj::Kernel::instance()->getClusterOptions();
	boost::program_options::variables_map vm;
	boost::program_options::store(boost::program_options::command_line_parser(argc, argv)
			.options(cluster_desc)
			.run(),
		vm);
	init(vm);
}

/// Fallback - if we have no other source of info
void KernelState::init() {
	VRJLUA_MSG_START(dbgVRJLUA, MSG_WARNING)
		<< "Warning: your application is not cluster-capable as compiled (VR Juggler 3.x) because you did not initialize the kernel with command line arguments!"
		<< VRJLUA_MSG_END(dbgVRJLUA, MSG_WARNING);
	boost::program_options::variables_map vm;
	// Juggler assumes these will be in a provided variable map, and gives an unhelpful boost::any_cast exception
	// if not set.
	vm.insert(std::make_pair(std::string("vrjmaster"), boost::program_options::variable_value(false, true)));
	vm.insert(std::make_pair(std::string("vrjslave"),boost::program_options::variable_value(false, true)));
	init(vm);
}

void KernelState::initAsSingleMachine() {
	boost::program_options::variables_map vm;
	vm.insert(std::make_pair(std::string("vrjmaster"), boost::program_options::variable_value(false, true)));
	vm.insert(std::make_pair(std::string("vrjslave"),boost::program_options::variable_value(false, true)));
	init(vm);
}

void KernelState::initAsClusterPrimaryNode() {
	boost::program_options::variables_map vm;
	vm.insert(std::make_pair(std::string("vrjmaster"), boost::program_options::variable_value(true, false)));
	vm.insert(std::make_pair(std::string("vrjslave"),boost::program_options::variable_value(false, true)));
	init(vm);
}

void KernelState::initAsClusterSecondaryNode(int port) {
	boost::program_options::variables_map vm;
	vm.insert(std::make_pair(std::string("vrjmaster"), boost::program_options::variable_value(false, true)));
	vm.insert(std::make_pair(std::string("vrjslave"),boost::program_options::variable_value(true, false)));
	if (port != 0) {
		vm.insert(std::make_pair(std::string("listen_port"),boost::program_options::variable_value(port, false)));
	}
	init(vm);
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

void KernelState::initAsSingleMachine() {
	_isCluster = false;
	_isPrimary = false;
	_init = true;
}

void KernelState::initAsClusterPrimaryNode() {
	_isCluster = true;
	_isPrimary = true;
	_init = true;
}

void KernelState::initAsClusterSecondaryNode(int /*port*/) {
	_isCluster = true;
	_isPrimary = false;
	_init = true;
}
#endif

namespace Kernel {
	void initAsClusterSecondaryNodeDefaultPort() {
		KernelState::initAsClusterSecondaryNode();
	}

	void doStart(bool waiting = false) {
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
	
	void start() {
		doStart();
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
		doStart(true);
		waitForKernelStop();	
	}

	void setApplication(luabind::object app) {
		vrj::Kernel::instance()->setApplication(luabind::object_cast<KernelOsgApp*>(app));
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
		def("initAsClusterPrimaryNode", &KernelState::initAsClusterPrimaryNode),
		def("initAsClusterSecondaryNode", &KernelState::initAsClusterSecondaryNode),
		def("initAsClusterSecondaryNode", &Kernel::initAsClusterSecondaryNodeDefaultPort),
		def("initAsSingleMachine", &KernelState::initAsSingleMachine)
		
	];

}

}// end of vrjLua namespace
