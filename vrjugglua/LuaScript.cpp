/**	@file
	@brief	implementation

	@date
	2009-2011

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

//          Copyright Iowa State University 2009-2011.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


// Internal Includes
#include "LuaScript.h"
#include "VRJLua_C_Interface.h"

#include "OsgAppProxy.h"
#include "LuaPath.h"
#include "LuaGCBlock.h"

#include "VRJLuaOutput.h"

#include "osgLuaBind.h"
#include "binding_detail/BindOsgToLua.inl"

#include "binding_detail/BindKernelToLua.inl"
#include "binding_detail/BindSonixToLua.inl"
#include "binding_detail/BindGadgetInterfacesToLua.inl"
#include "binding_detail/BindRunBufferToLua.inl"
#include "binding_detail/BindvrjLuaToLua.inl"

// Library/third-party includes
#include <vrjugglua/LuaIncludeFull.h>

#include <luabind/luabind.hpp>
#include <luabind/class_info.hpp>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

// Standard includes
#include <functional>
#include <iostream>
#include <sstream>

namespace vrjLua {

/// @brief no-op deleter for externally-provided state pointers
	static void no_op_deleter(lua_State *L) {
		return;
	}

	bool LuaScript::exitOnError = false;
	boost::function<void (std::string const&)> LuaScript::_printFunc;

	LuaScript::LuaScript(const bool create) {
#ifdef VERBOSE
		std::cout << "In constructor " << __FUNCTION__ << " at " << __FILE__ << ":" << __LINE__ << " with this=" << this << std::endl;
#endif
		if (create) {
#ifdef VERBOSE
			std::cout << "In constructor " << __FUNCTION__ << " at " << __FILE__ << ":" << __LINE__ << " with this=" << this << std::endl;
#endif
			_state = LuaStatePtr(luaL_newstate(), std::ptr_fun(lua_close));

			if (!_state) {
				std::cerr << "Warning: Could not allocate a new Lua state in the LuaScript constructor!" << std::endl;
				return;
			}

			{
				/// RAII-style disabling of garbage collector during init
				LuaGCBlock gcBlocker(_state.get());
				// Load default Lua libs
				luaL_openlibs(gcBlocker.state());

				/// @todo Extend the path here for shared libraries?
				//luabind::call_function<std::string>(_state.get(), "format", "%q", )
				//luaL_dostring(_state.get(), "package.cpath = ")

				_applyBindings();
			}
		}
	}

	LuaScript::LuaScript(lua_State * state, bool bind) :
		_state(borrowStatePtr(state)) {
#ifdef VERBOSE
		std::cout << "In constructor " << __FUNCTION__ << " at " << __FILE__ << ":" << __LINE__ << " with this=" << this << std::endl;
#endif
		if (!state) {
			std::cerr << "Warning: constructing a LuaScript from a null state pointer!" << std::endl;
		}

		// If requested, bind.
		if (bind) {
			_applyBindings();
		}
	}

	LuaScript::LuaScript(const LuaScript & other) :
		_state(other._state) {
#ifdef VERBOSE
		std::cout << "**** LuaScript copy constructor invoked" << std::endl;
#endif
#ifdef VERBOSE
		std::cout << "In constructor " << __FUNCTION__ << " at " << __FILE__ << ":" << __LINE__ << " with this=" << this << std::endl;
#endif
	}

	LuaScript::LuaScript(const LuaStatePtr & otherptr) :
		_state(otherptr) {
		if (!otherptr) {
			std::cerr << "Warning: constructing a LuaScript from an empty state smart pointer!" << std::endl;
		}
#ifdef VERBOSE
		std::cout << "In constructor " << __FUNCTION__ << " at " << __FILE__ << ":" << __LINE__ << " with this=" << this << std::endl;
#endif
	}

	LuaScript::~LuaScript() {
#ifdef VERBOSE
		std::cout << "In destructor " << __FUNCTION__ << " at " << __FILE__ << ":" << __LINE__ << " with this=" << this << std::endl;
#endif
	}

	LuaScript & LuaScript::operator=(const LuaScript & other) {
		if (!other._state) {
			std::cerr << "Warning: setting a LuaScript equal to another LuaScript with an empty state smart pointer!" << std::endl;
		}

		if (&other == this) {
			/// self assignment
			return *this;
		}

		_state = other._state;
		return *this;
	}

	bool LuaScript::_handleLuaReturnCode(int returnVal, std::string const& failureMsg, std::string const& successMsg) {
		if (returnVal != 0) {
			std::string err;
			try {
				luabind::object o(luabind::from_stack(_state.get(), -1));
				err = "  Lua returned this error message: " + luabind::object_cast<std::string>(o);
			} catch (...) {
				// do nothing - couldn't get an error string
				err = "  Furthermore, we couldn't get error details from Lua.";
			}

			doPrint(failureMsg + err);
			return false;
		} else {
			if (!successMsg.empty()) {
				VRJLUA_MSG_START(dbgVRJLUA, MSG_STATUS)
				        << successMsg
				        << VRJLUA_MSG_END(dbgVRJLUA, MSG_STATUS);
			}
			return true;
		}
	}

	bool LuaScript::runFile(const std::string & fn, bool silentSuccess) {
		if (!_state) {
			throw NoValidLuaState();
		}

		return _handleLuaReturnCode(luaL_dofile(_state.get(), fn.c_str()),
		                            std::string("vrjLua ERROR: Could not run Lua file ") + fn + ".",
		                            silentSuccess ? "" : std::string("Successfully ran Lua file ") + fn);
	}

	bool LuaScript::requireModule(const std::string & mod, bool silentSuccess) {
		if (!_state) {
			throw NoValidLuaState();
		}
		try {
			luabind::call_function<void>(_state.get(), "require", mod);
		} catch (std::exception & e) {
			doPrint(std::string("vrjLua ERROR: Could not load Lua module ") + mod + " - error: " + e.what());
			return false;
		}
		if (!silentSuccess) {
			VRJLUA_MSG_START(dbgVRJLUA, MSG_STATUS)
			        << "Successfully loaded Lua module " << mod
			        << VRJLUA_MSG_END(dbgVRJLUA, MSG_STATUS);
		}
		return true;
	}

	bool LuaScript::runString(const std::string & str, bool silentSuccess) {
		if (!_state) {
			throw NoValidLuaState();
		}

		return _handleLuaReturnCode(luaL_dostring(_state.get(), str.c_str()),
		                            "vrjLua ERROR: Could not run provided Lua string.",
		                            silentSuccess ? "" : "Lua string executed successfully.");
	}

	void LuaScript::_applyBindings() {
		if (!_state) {
			throw NoValidLuaState();
		}
		// Connect LuaBind to this state
		try {
			luabind::open(_state.get());
			luabind::bind_class_info(_state.get());
		} catch (const std::exception & e) {
			std::cerr << "Caught exception connecting luabind and class_info: " << e.what() << std::endl;
			throw;
		}

		/// Apply our bindings to this state

		// Extend the lua script search path for "require"
		LuaPath& lp = LuaPath::instance();
		lp.updateLuaRequirePath(_state);

		// osgLua
		bindOsgToLua(_state);

		// vrjugglua
		bindKernelToLua(_state);
		bindSonixToLua(_state);
		bindGadgetInterfacesToLua(_state);
		bindRunBufferToLua(_state);
		BindvrjLuaToLua(_state);

		OsgAppProxy::bindToLua(_state);

		// Set up traceback...
		luabind::set_pcall_callback(&add_file_and_line);

		// Load the vrjlua init script
		requireModule("vrjlua-init", true);

		// set up global for debug mode
		/// @todo make this work
		/*
		luabind::module(_state.get(), "vrjlua")[
											   def_readwrite(&LuaScript::exitOnError)
											   ];
		*/

	}

	bool LuaScript::call(const std::string & func, bool silentSuccess) {
		if (!_state) {
			throw NoValidLuaState();
		}
		try {
			return luabind::call_function<bool>(_state.get(), func.c_str());

			if (!silentSuccess) {
				VRJLUA_MSG_START(dbgVRJLUA, MSG_STATUS)
				        << "Successfully called Lua function  " << func
				        << VRJLUA_MSG_END(dbgVRJLUA, MSG_STATUS);
			}
		} catch (const std::exception & e) {
			std::cerr << "Caught exception calling '" << func << "': " << e.what() << std::endl;
			throw;
		}
	}

	void LuaScript::setPrintFunction(boost::function<void (std::string const&)> func) {
		_printFunc = func;
	}

	LuaStateWeakPtr LuaScript::getLuaState() const {
		return _state;
	}


	LuaStateRawPtr LuaScript::getLuaRawState() const {
		return _state.get();
	}

	boost::program_options::options_description LuaScript::getVrjOptionsDescriptions() {
		return KernelState::getVrjOptionsDescriptions();
	}

	void LuaScript::initVrjKernel(boost::program_options::variables_map const& vm) {
		KernelState::init(vm);
	}

	void LuaScript::initVrjKernel(int argc, char* argv[]) {
		KernelState::init(argc, argv);
	}

	void LuaScript::initVrjKernelAsSingleMachine() {
		KernelState::initAsSingleMachine();
	}

	void LuaScript::initVrjKernelAsClusterPrimary() {
		KernelState::initAsClusterPrimaryNode();
	}
	void LuaScript::initVrjKernelAsClusterSecondary(int port) {
		KernelState::initAsClusterSecondaryNode(port);
	}

	void LuaScript::doPrint(std::string const& str) {
		if (_printFunc) {
			_printFunc(str);
		} else {
			VRJLUA_MSG_START(dbgVRJLUA_APP, MSG_STATUS)
			        << str
			        << VRJLUA_MSG_END(dbgVRJLUA_APP, MSG_STATUS);
		}
	}


	void LuaPath::_populateSearchPathsVector(LuaStatePtr state) {
		luabind::object package = luabind::globals(state.get())["package"];
		std::string input = luabind::object_cast<std::string>(package["path"]);
		boost::algorithm::split(_searchPaths, input, boost::is_any_of(";"));

		// Remove the items we'll add ourselves.
		std::deque<std::string>::iterator it = std::find(_searchPaths.begin(), _searchPaths.end(), "?");
		while (it != _searchPaths.end()) {
			_searchPaths.erase(it);
			it = std::find(_searchPaths.begin(), _searchPaths.end(), "?");
		}

		it = std::find(_searchPaths.begin(), _searchPaths.end(), "?.lua");
		while (it != _searchPaths.end()) {
			_searchPaths.erase(it);
			it = std::find(_searchPaths.begin(), _searchPaths.end(), "?.lua");
		}

		_searchPaths.push_front(_luaDir + "?.lua");
		_searchPaths.push_front(_luaDir + "?");
	}

	void LuaPath::_setLuaSearchPaths(LuaStatePtr state) {
		std::ostringstream scr;
		scr << "?;";
		scr << "?.lua;";
		for (unsigned int i = 0; i < _searchPaths.size(); ++i) {
			scr << _searchPaths[i] << ";";
		}
		luabind::object package = luabind::globals(state.get())["package"];
		package["path"] = scr.str();
	}

} // end of vrjLua namespace
