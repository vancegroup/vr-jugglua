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
#pragma once
#ifndef INCLUDED_vrjugglua_LuaScript_h
#define INCLUDED_vrjugglua_LuaScript_h

// Local includes
#include <vrjugglua/VRJLua_C_Interface.h>

// Library/third-party includes
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/program_options.hpp>

// Standard includes
#include <string>
#include <stdexcept>

namespace vrjLua {

typedef lua_State * LuaStateRawPtr;
typedef boost::shared_ptr<lua_State> LuaStatePtr;
typedef boost::weak_ptr<lua_State> LuaStateWeakPtr;

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

		bool runFile(const std::string & fn);
		bool runString(const std::string & str);
		bool requireModule(const std::string & mod);
		bool call(const std::string & func);

		LuaStateWeakPtr getLuaState() const;

		bool isValid() const;

		static bool exitOnError;

		static boost::program_options::options_description getVrjOptionsDescriptions();
		static void initVrjKernel(boost::program_options::variables_map const& vm);
		static void initVrjKernel(int argc, char* argv[]);

	protected:
		void _applyBindings();
		LuaStatePtr _state;
};

// -- inline implementations -- /

inline bool LuaScript::isValid() const {
	return (_state);
}

}// end of vrjLua namespace

#endif // INCLUDED_vrjugglua_LuaScript_h
