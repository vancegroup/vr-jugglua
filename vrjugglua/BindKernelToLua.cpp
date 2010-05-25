/**	@file	BindKernelToLua.cpp
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

// Local includes
#include "BindKernelToLua.h"

// Library/third-party includes
#include <luabind/luabind.hpp>

// Standard includes
// - none

namespace vrjLua {

void bindKernelToLua(LuaStatePtr state) {
	using namespace luabind;
	module(state.get(), "vrj") [
		def("stopKernel", &Internal::stopKernel),
		def("setApplication", &Internal::setApplication)
	];
}

}// end of vrjLua namespace
