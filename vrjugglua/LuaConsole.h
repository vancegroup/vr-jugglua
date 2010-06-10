/**	@file	LuaConsole.h
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
#ifndef INCLUDED_vrjugglua_LuaConsole_h
#define INCLUDED_vrjugglua_LuaConsole_h

// Local includes
#include <vrjugglua/LuaScript.h>
#include <vrjugglua/SynchronizedRunBuffer.h>

// Library/third-party includes
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

// Standard includes
#include <string>

namespace vrjLua {

class LuaConsole {
	public:
		LuaConsole();
		LuaConsole(LuaScript const& script);

		virtual ~LuaConsole();

		bool getRunBufFromLuaGlobal();
		bool createRunBuf();

		/// @name Implementation interface
		/// @{
		virtual bool threadLoop() = 0;

		virtual void stopThread() = 0;

		virtual void appendToDisplay(std::string const& message) = 0;

		virtual void setTitle(std::string const& title) = 0;
		/// @}

		bool isValid() const;

		bool runFile(std::string const& fn);
		bool runString(std::string const& str);

	protected:

		LuaScript _script;

		boost::shared_ptr<SynchronizedRunBuffer> _runbuf;
};

class StubConsole : public LuaConsole {
	public:
		StubConsole();
		StubConsole(LuaScript const& script);

		virtual ~StubConsole();

		/// @name Implementation interface
		/// @{
		virtual bool threadLoop();

		virtual void stopThread();

		virtual void appendToDisplay(std::string const& message);

		virtual void setTitle(std::string const& title);
		/// @}
};

// -- inline implementations -- /

}// end of vrjLua namespace

#endif // INCLUDED_vrjugglua_LuaConsole_h
