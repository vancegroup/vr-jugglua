/**	@file	BindKernelToLua.h
	@brief	header

	@date
	2009-2010

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/
#pragma once
#ifndef INCLUDED_vrjugglua_BindKernelToLua_h
#define INCLUDED_vrjugglua_BindKernelToLua_h

// Local includes
#include "LuaScript.h"

// Library/third-party includes
#include <vrj/Kernel/Kernel.h>
#include <vrj/Draw/OSG/OsgApp.h>

#include <luabind/luabind.hpp>

// Standard includes
// - none

namespace vrjLua {

namespace Internal {
	inline void stopKernel() {
		vrj::Kernel::instance()->stop();
	}

	inline void setApplication(vrj::OsgApp * app) {
		vrj::Kernel::instance()->setApplication(app);
	}

}

void bindKernelToLua(LuaStatePtr state) {
	using namespace luabind;
	module(state.get(), "vrj") [
		def("stopKernel", &Internal::stopKernel),
		def("setApplication", &Internal::setApplication)
	];
}

}// end of vrjLua namespace


#endif // INCLUDED_vrjugglua_BindKernelToLua_h
