/**	@file
        @brief	header

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

#pragma once
#ifndef INCLUDED_vrjugglua_LuaScript_h
#define INCLUDED_vrjugglua_LuaScript_h

// Local includes
#include <vrjugglua/LuaStateFwd.h>

// Library/third-party includes
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

// Standard includes
#include <string>
#include <stdexcept>
#include <functional> // for std::ptr_fun

namespace vrjLua {
    /// @brief no-op deleter for externally-provided state pointers
    inline void state_no_op_deleter(lua_State * /*L*/) { return; }

    typedef boost::shared_ptr<lua_State> LuaStatePtr;

    inline LuaStatePtr borrowStatePtr(lua_State *ptr) {
        return LuaStatePtr(ptr, std::ptr_fun(state_no_op_deleter));
    }

    struct CouldNotOpenState : public std::runtime_error {
        CouldNotOpenState()
            : std::runtime_error("Could not open a new Lua state.") {}
    };

    struct NoValidLuaState : public std::logic_error {
        NoValidLuaState()
            : std::logic_error("Attempted to perform a LuaScript manipulation "
                               "on a LuaScript instance without a valid state "
                               "pointer!") {}
    };

    class LuaScript {
      public:
        /// @brief Default constructor, that can optionally not allocate a new
        /// state.
        explicit LuaScript(const bool create = true);

        /// @brief constructor from an externally-allocated state
        LuaScript(lua_State *state, bool bind = false);

        /// @brief copy constructor - doesn't re-bind. Will not copy from an
        /// empty pointer.
        LuaScript(const LuaScript &other);

        /// @brief copy constructor - doesn't re-bind. Will not copy from an
        /// empty pointer.
        LuaScript(const LuaStatePtr &other);

        ~LuaScript();

        LuaScript &operator=(const LuaScript &other);

        static void initWithArgv0(const char *argv0);

        bool runFile(const std::string &fn, bool silentSuccess = true);
        bool runString(const std::string &str, bool silentSuccess = false);
        bool requireModule(const std::string &mod, bool silentSuccess = true);
        bool call(const std::string &func, bool silentSuccess = true);

        void setPrintFunction(boost::function<void(std::string const &)> func);
        static void doPrint(std::string const &str);

        /// Gets a shared pointer to the Lua state, guaranteed to be non-null.
        LuaStatePtr const &getLuaState() const;

        /// Gets a raw pointer to the Lua state, guaranteed to be non-null
        lua_State *getLuaRawState() const;

        bool isValid() const;

        static bool exitOnError;

      protected:
        static boost::function<void(std::string const &)> _printFunc;
        bool _handleLuaReturnCode(int returnVal, std::string const &failureMsg,
                                  std::string const &successMsg = "");
        void _applyBindings();
        LuaStatePtr _state;
    };

    // -- inline implementations -- /

    inline bool LuaScript::isValid() const { return (_state); }

} // end of vrjLua namespace

#endif // INCLUDED_vrjugglua_LuaScript_h
