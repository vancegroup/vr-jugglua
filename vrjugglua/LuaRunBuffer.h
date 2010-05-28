/**	@file	LuaRunBuffer.h
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
#ifndef INCLUDED_vrjugglua_LuaRunBuffer_h
#define INCLUDED_vrjugglua_LuaRunBuffer_h

// Local includes
#include <vrjugglua/LuaScript.h>

// Library/third-party includes
#include <boost/circular_buffer.hpp>

#include <vpr/Sync/Mutex.h>

// Standard includes
#include <string>

namespace vrjLua {

class LuaRunBuffer {
	public:
		LuaRunBuffer();

		/// @name Adding new chunks to the run buffer
		/// @{
		bool addFile(const std::string & fn, bool blocking = false);
		bool addString(const std::string & str, bool blocking = false);
		/// @}
		
		/// @brief Run currently buffered chunks - only call from a single thread!
		int runBuffer(LuaScript & script);

	protected:
		boost::circular_buffer<std::string> _buf;
		vpr::Mutex _lock;
};

// -- inline implementations -- /

}// end of vrjLua namespace


#endif // INCLUDED_vrjugglua_LuaScript_h
