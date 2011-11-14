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
#ifndef INCLUDED_vrjugglua_LuaConsole_h
#define INCLUDED_vrjugglua_LuaConsole_h

// Local includes
#include <vrjugglua/LuaScript.h>

// Library/third-party includes
#include <boost/shared_ptr.hpp>

// Standard includes
#include <string>
#include <sstream>

namespace vrjLua {
	class SynchronizedRunBuffer;
	class LuaConsole;

	namespace detail {
		class LuaConsoleInitialOutputProxy;
		class LuaConsoleOutputProxy {
			public:
				template<typename T>
				std::ostream & operator<<(T const& val) {
					_stream << val;
					return _stream;
				}

				~LuaConsoleOutputProxy();
			private:
				friend class LuaConsoleInitialOutputProxy;
				template<typename T>
				LuaConsoleOutputProxy(LuaConsole * console, T const& val)
					: _console(console)
				{
					_stream << val;
				}

				LuaConsole * _console;
				std::ostringstream _stream;
		};

		class LuaConsoleInitialOutputProxy {
			public:
				template<typename T>
				std::ostream & operator<<(T const& val) {
					return LuaConsoleOutputProxy(_console, val);
				}


			private:
				friend class LuaConsole;
				LuaConsoleInitialOutputProxy(LuaConsole * console)
					: _console(console)
					{}
				LuaConsole * _console;

		};
	} // end of namespace detail
	class LuaConsole {
		public:
			LuaConsole();
			LuaConsole(LuaScript const& script);

			virtual ~LuaConsole();

			bool getRunBufFromLuaGlobal();
			bool createRunBuf();

			void captureStdOut();
			void captureStdErr();

			/** @brief This function is used for a "cout"-style way of
				putting text in the console.

				Use like this:
				console->appendToDisplay() << "This is some text " << 5;
			*/
			detail::LuaConsoleInitialOutputProxy appendToDisplay() {
				return detail::LuaConsoleInitialOutputProxy(this);
			}


			/// @name Implementation interface
			/// @{
			virtual bool threadLoop() = 0;

			virtual void stopThread() = 0;

			virtual void appendToDisplay(std::string const& message) = 0;

			virtual void setTitle(std::string const& title) = 0;

			virtual void disableAction() = 0;
			/// @}

			bool isValid() const;

			virtual bool supportsAlternateLogging() const {
				return false;
			}

			bool addFile(std::string const& fn);
			bool addString(std::string const& str);

			bool runFileImmediately(std::string const& fn);
			bool runStringImmediately(std::string const& str);

			bool requireModuleImmediately(std::string const& module);

			bool runBuffer();
			LuaScript& getScript();

			static LuaConsole * getConsole();

		protected:
			/// Call from subclasses to switch print function to the GUI Console print.
			void _consoleIsReady();
			static LuaConsole * s_console;

			bool _loggingActive;

			std::ostringstream _log;

			LuaScript _script;

			boost::shared_ptr<SynchronizedRunBuffer> _runbuf;
	};

	class StubConsole : public LuaConsole {
		public:
			StubConsole();
			StubConsole(LuaScript const& script);

			virtual ~StubConsole();

			static void setup(int & argc, char * argv[]);

			/// @name Implementation interface
			/// @{
			virtual bool threadLoop();

			virtual void stopThread();

			virtual void appendToDisplay(std::string const& message);

			virtual void setTitle(std::string const& title);

			virtual void disableAction();
			/// @}
	};

// -- inline implementations -- /

}// end of vrjLua namespace

#endif // INCLUDED_vrjugglua_LuaConsole_h
