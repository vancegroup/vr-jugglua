/**	@file	SynchronizedRunBuffer.h
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
#ifndef INCLUDED_vrjugglua_SynchronizedRunBuffer_h
#define INCLUDED_vrjugglua_SynchronizedRunBuffer_h

// Local includes
#include "LuaScript.h"
#include "LuaRunBuffer.h"

// Library/third-party includes
#include <plugins/ApplicationDataManager/UserData.h>

#ifdef NEED_RUNBUFFER_LUABIND_OBJECT
#include <luabind/object.hpp>
#endif

// Standard includes
#include <vector>
#include <string>
#include <utility>

namespace vrjLua {

	class SynchronizedRunBuffer {
		public:
			/// @name Constructors
			/// They accept different arguments as ways to get to the Lua state.
			/// @{
#ifdef NEED_RUNBUFFER_LUABIND_OBJECT
			SynchronizedRunBuffer(luabind::object const& delegate);
#endif
			SynchronizedRunBuffer(LuaStatePtr const& state);
			SynchronizedRunBuffer(LuaScript const& script);
			/// @}

			/// @brief Initialize application shared data, in initScene
			void init();

			/// @brief are we the data-local (master) node?
			bool isLocal();

			/// @name Methods that only make sense on the data-local (master) node
			/// @{
			bool addFile(std::string const& filename, bool blocking = false);
			bool addString(std::string const& str, bool blocking = false);
			/// @}

			/** @brief Buffer execution - should take place in latePreFrame
			 */
			unsigned int runBuffer();

		protected:
			bool _init;
			cluster::UserData<LuaRunBuffer> _runBuf;
			enum COMMAND {
				CM_ADDFILE,
				CM_ADDSTRING
			};
			typedef std::pair<COMMAND, std::string> RunBufCmd;

			std::vector<RunBufCmd> _earlyBirds;
			lua_State * _state;

			void _checkInit();
	};

} // end of vrjLua namespace



#endif // INCLUDED_vrjugglua_SynchronizedRunBuffer_h
