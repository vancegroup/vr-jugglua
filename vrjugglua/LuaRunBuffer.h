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
#ifndef INCLUDED_vrjugglua_LuaRunBuffer_h
#define INCLUDED_vrjugglua_LuaRunBuffer_h

// Local includes
#include <vrjugglua/LuaScript.h>

// Library/third-party includes
#include <boost/circular_buffer.hpp>

#include <vpr/Sync/CondVar.h>
#include <vpr/IO/SerializableObject.h>
#include <vpr/Util/GUID.h>

// Standard includes
#include <string>

namespace vrjLua {

	class LuaRunBuffer : public vpr::SerializableObject {
		public:
			LuaRunBuffer(unsigned int capacity = 10, bool runBlocks = false);
			~LuaRunBuffer();

			/// @brief sets the lua state
			void initLua(lua_State * L);

			/// @brief Checks to see if we've been initialized
			bool ready() const;

			/// @brief Get the GUID for a single RunBuffer synchronized in an app
			static vpr::GUID getGUID();

			/// @name Adding new chunks to the run buffer
			/// @{
			bool addFile(const std::string & filename, bool blocking = false);
			bool addString(const std::string & str, bool blocking = false);
			/// @}

			/// @brief Run currently buffered chunks - only call from a single thread!
			unsigned int runBuffer();



			/// @name vpr::SerializableObject interface
			/// @{
			void writeObject(vpr::ObjectWriter* writer);
			void readObject(vpr::ObjectReader* reader);
			/// @}

		protected:
			bool _full() const;
			bool _empty() const;

			LuaScript _script;

			boost::circular_buffer<std::string> _buf;
			vpr::CondVar _cond;
			unsigned int _maxRun;

			/// @brief configuration - whether we should block in runBuffer
			bool _runBlock;
	};

// -- inline implementations -- /

}// end of vrjLua namespace


#endif // INCLUDED_vrjugglua_LuaScript_h
