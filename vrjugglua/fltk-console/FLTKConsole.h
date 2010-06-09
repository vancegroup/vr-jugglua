/**	@file	FLTKConsole.h
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
#ifndef INCLUDED_vrjugglua_fltk_console_FLTKConsole_h
#define INCLUDED_vrjugglua_fltk_console_FLTKConsole_h

// Local includes
#include <vrjugglua/LuaConsole.h>

// Library/third-party includes
#include <vpr/Thread/Thread.h>

// Standard includes
// - none

class FLTKConsoleUI;

namespace vrjLua {

class FLTKConsole : public LuaConsole {
	public:
		FLTKConsole();
		FLTKConsole(LuaScript const& script);

		virtual ~FLTKConsole();

		/// @name Interface required by LuaConsole
		/// @{
		virtual bool startThread();

		virtual void stopThread();

		virtual void waitForThreadStop();

		virtual void appendToDisplay(std::string const& message);

		virtual void setTitle(std::string const& title);
		/// @}

		static void threadEntryPoint();
		void _threadLoop();

	protected:
		bool _doThreadWork();

		bool _running;
		vpr::Thread _thread;

		boost::shared_ptr<FLTKConsoleUI> _view;

		static FLTKConsole * s_console;
};

// -- inline implementations -- /

}// end of vrjLua namespace

#endif // INCLUDED_vrjugglua_fltk_console_FLTKConsole_h
