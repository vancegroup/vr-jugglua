/**	@file
	@brief	header for an FLTK-based GUI console

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
#ifndef INCLUDED_vrjugglua_fltk_console_FLTKConsole_h
#define INCLUDED_vrjugglua_fltk_console_FLTKConsole_h

// Local includes
#include <vrjugglua/LuaConsole.h>
#include <vrjugglua/RunLoopManager.h>

// Library/third-party includes
#include <boost/utility.hpp> // for boost::noncopyable

// Standard includes
// - none

class FLTKConsoleUI;

namespace vrjLua {

	class FLTKConsole : public LuaConsole, boost::noncopyable {
		public:
			FLTKConsole();
			FLTKConsole(LuaScript const& script);

			static void setup(int & argc, char * argv[]);

			virtual ~FLTKConsole();

			/// @name Interface required by LuaConsole
			/// @{
			virtual bool threadLoop();

			virtual void stopThread();

			virtual void appendToDisplay(std::string const& message);

			virtual void setTitle(std::string const& title);

			virtual void disableAction();
			/// @}

		protected:
			bool _doThreadWork();

			RunLoopManager run_;

			boost::shared_ptr<FLTKConsoleUI> _view;
	};

// -- inline implementations -- /

}// end of vrjLua namespace

#endif // INCLUDED_vrjugglua_fltk_console_FLTKConsole_h
