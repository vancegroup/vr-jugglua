/**	@file	LuaScript.h
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

//          Copyright Iowa State University 2010-2011.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#ifndef INCLUDED_vrjugglua_LuaScript_h
#define INCLUDED_vrjugglua_LuaScript_h

// Local includes
#include <vrjugglua/LuaInclude.h>

// Library/third-party includes
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

// Standard includes
#include <string>
#include <stdexcept>

namespace vrjLua {

	inline void state_no_op_deleter(lua_State * /*L*/) {
		return;
	}

	typedef lua_State * LuaStateRawPtr;
	typedef boost::shared_ptr<lua_State> LuaStatePtr;
	typedef boost::weak_ptr<lua_State> LuaStateWeakPtr;

	inline LuaStatePtr borrowStatePtr(LuaStateRawPtr ptr) {
		return LuaStatePtr(ptr, std::ptr_fun(state_no_op_deleter));
	}

	struct NoValidLuaState : public std::logic_error {
		NoValidLuaState() : std::logic_error("Attempted to perform a LuaScript manipulation on a LuaScript instance without a valid state pointer!") {}
	};

	class LuaScript {
		public:
			/// @brief Default constructor, that can optionally not allocate a new state.
			explicit LuaScript(const bool create = true);

			/// @brief constructor from an externally-allocated state
			LuaScript(lua_State * state, bool bind = false);

			/// @brief copy constructor - doesn't re-bind
			LuaScript(const LuaScript & other);

			/// @brief copy constructor - doesn't re-bind
			LuaScript(const LuaStatePtr & other);

			~LuaScript();

			LuaScript & operator=(const LuaScript & other);

			bool runFile(const std::string & fn, bool silentSuccess = true);
			bool runString(const std::string & str, bool silentSuccess = false);
			bool requireModule(const std::string & mod, bool silentSuccess = true);
			bool call(const std::string & func, bool silentSuccess = true);

			void setPrintFunction(boost::function<void (std::string const&)> func);
			static void doPrint(std::string const& str);

			LuaStateWeakPtr getLuaState() const;
			LuaStateRawPtr getLuaRawState() const;

			bool isValid() const;

			static bool exitOnError;

			static boost::program_options::options_description getVrjOptionsDescriptions();
			static void initVrjKernel(boost::program_options::variables_map const& vm);
			static void initVrjKernel(int argc, char* argv[]);

			static void initVrjKernelAsSingleMachine();
			static void initVrjKernelAsClusterPrimary();
			static void initVrjKernelAsClusterSecondary(int port = 0);

		protected:
			static boost::function<void (std::string const&)> _printFunc;
			bool _handleLuaReturnCode(int returnVal, std::string const& failureMsg, std::string const& successMsg = "");
			void _applyBindings();
			LuaStatePtr _state;
	};

// -- inline implementations -- /

	inline bool LuaScript::isValid() const {
		return (_state);
	}

}// end of vrjLua namespace

#endif // INCLUDED_vrjugglua_LuaScript_h
