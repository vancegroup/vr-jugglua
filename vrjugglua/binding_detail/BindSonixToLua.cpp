/**	@file	binding_detail/BindSonixToLua.cpp
	@brief	implementation of Sonix binding

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
#include "BindSonixToLua.h"
#include "../VRJLuaOutput.h"

// Library/third-party includes
#include <luabind/luabind.hpp>

#include <snx/SoundHandle.h>

// Standard includes
#ifdef VERBOSE
#include <iostream>
#endif

namespace vrjLua {
using namespace luabind;

namespace Sonix {
	void changeAPI(const std::string & api) {
		snx::sonix::instance()->changeAPI(api);
	}
} // end of Internal namespace

void bindSonixToLua(LuaStatePtr state) {
#ifdef VERBOSE
	std::cerr << "Registering snx module functions and objects with Lua..." << std::flush << std::endl;
#endif
	module(state.get(), "snx") [
		def("changeAPI", &Sonix::changeAPI),

		class_<snx::SoundInfo>("SoundInfo")
			.def_readwrite("alias", &snx::SoundInfo::alias)
			.def_readwrite("filename", &snx::SoundInfo::filename)
			.def_readwrite("ambient", &snx::SoundInfo::ambient)
			.def_readwrite("repeat", &snx::SoundInfo::repeat)
			.def_readwrite("volume", &snx::SoundInfo::volume)
			.def_readwrite("streaming", &snx::SoundInfo::streaming)
		,

		class_<snx::SoundHandle>("SoundHandle")
			.def(constructor<const std::string &>())
			.def("configure", &snx::SoundHandle::configure)
			.def("remove", &snx::SoundHandle::remove)
			.def_readonly("name", &snx::SoundHandle::getName)
			.def("trigger", &snx::SoundHandle::trigger)
			.def_readonly("isPlaying", &snx::SoundHandle::isPlaying)
			.def("stop", &snx::SoundHandle::stop)
			.def("pause", &snx::SoundHandle::pause)
			.def("unpause", &snx::SoundHandle::unpause)
			.def_readonly("isPaused", &snx::SoundHandle::isPaused)
			.property("ambient", &snx::SoundHandle::isAmbient, &snx::SoundHandle::setAmbient)
			.property("retriggerable", &snx::SoundHandle::isRetriggerable, &snx::SoundHandle::setRetriggerable)
			.def("setPosition", &snx::SoundHandle::setPosition)
	];

}

}// end of vrjLua namespace
