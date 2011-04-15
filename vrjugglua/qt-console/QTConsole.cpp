/**	@file	qt-console/QTConsole.cpp
	@brief	implementation of QT console GUI

	@date
	2009-2010

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

// Internal Includes
#include "QTConsole.h"


// uic-generated source
#include <ui_QTConsoleMainWindow.h>

// Library/third-party includes
#include <boost/scoped_ptr.hpp>

#include <QFileDialog>
#include <QScrollBar>
#include <QTimer>
#include <QFontDatabase>

#include <osgDB/FileUtils>

#include <vrj/Kernel/Kernel.h>

// Standard includes
#include <iostream>
#include <fstream>
#include <stdexcept>


namespace vrjLua {

QApplication* QTConsole::s_app = NULL;

static const int POLLING_INTERVAL = 100;

static const int LOG_UPDATE_INTERVAL = 250;

void QTConsole::setup(int & argc, char * argv[]) {
	s_app = new QApplication(argc, argv);
}

QTConsole::QTConsole() :
		_app(s_app),
		_running(false),
		_ui(new Ui::MainWindow()){
	_shared_init();
}

QTConsole::QTConsole(LuaScript const& script) :
		LuaConsole(script),
		_app(s_app),
		_running(false),
		_ui(new Ui::MainWindow()){
	_shared_init();
}

QTConsole::QTConsole(QApplication* app) :
		_app(app),
		_running(false),
		_ui(new Ui::MainWindow()) {
	_shared_init();
}

QTConsole::QTConsole(QApplication* app, LuaScript const& script) :
		LuaConsole(script),
		_app(app),
		_running(false),
		_ui(new Ui::MainWindow()) {
	_shared_init();
}

void QTConsole::_shared_init() {
	assert(_app);
	_ui->setupUi(this);
	_ui->plainTextDebugLog->hide();
	bool needsDroid = true;
#ifdef _WIN32
	/// If windows has consolas, we'll use that
	QFont consolas("Consolas", 10);
	if (consolas.exactMatch()) {
		needsDroid = false;
	}
#endif

	if (!needsDroid) {
		return;
	}

	/// Try to set font to Droid Sans Mono Slashed 8pt
	std::string fontFn = osgDB::findDataFile("assets/fonts/droid-sans-mono-slashed/DroidSansMonoSlashed.ttf");
	if (fontFn.empty()) {
		return;
	}

	int id = QFontDatabase::addApplicationFont(QString::fromStdString(fontFn));
	if (id == -1) {
		return;
	}

	// font load succeeded
	QFont droid("Droid Sans Mono Slashed", 8);
	_ui->plainTextDebugLog->setFont(droid);
	_ui->plainTextEdit->setFont(droid);
	_ui->plainTextEditLog->setFont(droid);
}

QTConsole::~QTConsole() {

}

void QTConsole::on_actionFileOpen_triggered() {
	QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open VRJ Lua File..."),
            QString(),
            tr("VRJ Lua Files (*.vrjlua *.lua);;All Files (*)"));

	if (!fileName.isEmpty() && !fileName.isNull()) {
		addFile(fileName.toStdString());
	}
}

void QTConsole::on_actionFileSave_triggered() {

	QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save VRJ Lua File..."),
            QString(),
            tr("VRJ Lua Files (*.vrjlua *.lua);;All Files (*)"));

	if (!fileName.isEmpty() && !fileName.isNull()) {
		std::ofstream df(fileName.toStdString().c_str());
		df << _ui->plainTextEdit->toPlainText().toStdString() << std::endl;
		df.close();
		std::string code("-- code up to here saved to file '");
		code += fileName.toStdString();
		code += "'\n";
		appendToDisplay(code);
	}

}

void QTConsole::on_actionFileExit_triggered() {
	close();
}

void QTConsole::on_buttonRun_clicked() {
	std::string code = _ui->plainTextEdit->toPlainText().toStdString();
	_ui->plainTextEdit->setPlainText(QString());
	addString(code);
}

void QTConsole::updateDebugLog() {
	QString newText = QString::fromStdString(_log.str());
	if (newText != _ui->plainTextDebugLog->toPlainText()) {
		_ui->plainTextDebugLog->setPlainText(newText);
		_ui->plainTextDebugLog->moveCursor(QTextCursor::End);
		_ui->plainTextDebugLog->ensureCursorVisible();
	}
}

void QTConsole::checkRunningState() {
	if (!_running || !vrj::Kernel::instance()->isRunning()) {
		close();
	}
}

void QTConsole::disableGUIAction() {
	_ui->plainTextEdit->setEnabled(false);
	_ui->buttonRun->setEnabled(false);
	_ui->actionFileOpen->setEnabled(false);
}

bool QTConsole::threadLoop() {
	if (_running) {
		/// @todo notify that the thread is already running?
		return false;
	}

	_running = true;

	boost::shared_ptr<QTimer> timer(new QTimer(this));
	connect(this, SIGNAL(textDisplaySignal(QString const&)), this, SLOT(addTextToDisplay(QString const&)));
	connect(this, SIGNAL(disableGUISignal()), this, SLOT(disableGUIAction()));
	connect(timer.get(), SIGNAL(timeout()), this, SLOT(checkRunningState()));
	timer->start(POLLING_INTERVAL);


	boost::shared_ptr<QTimer> logTimer(new QTimer(this));
	if (_loggingActive) {
		connect(logTimer.get(), SIGNAL(timeout()), this, SLOT(updateDebugLog()));
		logTimer->start(LOG_UPDATE_INTERVAL);
	} else {
		_ui->actionShow_debug_log->setEnabled(false);
	}

	/// Once GUI is ready, tell the console base class to redirect print statements.
	QTimer::singleShot(0, this, SLOT(consoleReady()));

	show();
	_app->exec();

	_running = false;
	vrj::Kernel * kern = vrj::Kernel::instance();
	if (kern) {
		kern->stop();
		kern->waitForKernelStop();
	}
	return true;
}

void QTConsole::stopThread() {
	_running = false;
}

void QTConsole::appendToDisplay(std::string const& message) {
	Q_EMIT textDisplaySignal(QString::fromStdString(message));
}

void QTConsole::addTextToDisplay(QString const& message) {
	_ui->plainTextEditLog->appendPlainText(message);
}

void QTConsole::setTitle(std::string const& title) {
	setWindowTitle(QString::fromStdString(title));
}

void QTConsole::consoleReady() {
	_consoleIsReady();
}

void QTConsole::disableAction() {
	appendToDisplay("-- Disabling interaction console GUI on this node...");
	Q_EMIT disableGUISignal();
}

} // end of vrjLua namespace
