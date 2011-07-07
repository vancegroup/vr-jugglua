/**	@file	qt-console/QTConsole.h
	@brief	header for an Qt-based GUI console

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
#ifndef INCLUDED_vrjugglua_qt_console_QTConsole_h
#define INCLUDED_vrjugglua_qt_console_QTConsole_h

#define QT_NO_KEYWORDS

// Local includes
#include <vrjugglua/LuaConsole.h>
#include <vrjugglua/LuaScript.h>

// Library/third-party includes
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <QMainWindow>
#include <QApplication>

// Standard includes
#include <string>

namespace Ui {
	class MainWindow;
}

namespace vrjLua {


	class QTConsole : public QMainWindow, boost::noncopyable, public LuaConsole {

			Q_OBJECT

		public:

			QTConsole();
			QTConsole(LuaScript const& script);
			QTConsole(QApplication* app);
			QTConsole(QApplication* app, LuaScript const& script);

			static void setup(int & argc, char * argv[]);


			virtual ~QTConsole();

			/// @name Interface required by LuaConsole
			/// @{
			virtual bool threadLoop();

			virtual void stopThread();

			virtual void appendToDisplay(std::string const& message);

			virtual void setTitle(std::string const& title);

			virtual void disableAction();
			/// @}

			bool supportsAlternateLogging() const {
				return true;
			}



		Q_SIGNALS:
			void textDisplaySignal(QString const& message);
			void disableGUISignal();

		private Q_SLOTS:

			void on_actionFileOpen_triggered();
			void on_actionFileSave_triggered();
			void on_actionFileExit_triggered();
			void on_buttonRun_clicked();

			void checkRunningState();
			void updateDebugLog();
			void addTextToDisplay(QString const& message);
			void disableGUIAction();
			void consoleReady();

		protected:
			void _shared_init();
			QApplication * _app;
			bool _running;

			static QApplication * s_app;
			boost::shared_ptr<Ui::MainWindow> _ui;

	};

// -- inline implementations -- /

}// end of vrjLua namespace

#endif // INCLUDED_vrjugglua_qt_console_QTConsole_h
