/**	@file	fltk-console/FLTKConsole.cpp
	@brief	implementation of FLTK console GUI

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

#include <QFileDialog.h>

#include <vrj/Kernel/Kernel.h>

// Standard includes
#include <iostream>
#include <stdexcept>


namespace vrjLua {

QTConsole::QTConsole(QApplication* app) :
	_app(app),
	_running(false),
	_ui(new Ui::MainWindow()) {
		_ui->setupUi(this);
}

QTConsole::QTConsole(QApplication* app, LuaScript const& script):
	LuaConsole(script),
	_app(app),
	_running(false),
	_ui(new Ui::MainWindow()) {
		
		_ui->setupUi(this);
}

QTConsole::~QTConsole() {

}

void QTConsole::on_actionFileOpen_triggered() {
	QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open VRJ Lua File..."),
            QString(),
            tr("VRJ Lua Files (*.vrjlua;*.lua);;All Files (*)"));

	if (!fileName.isEmpty() && !fileName.isNull()) {
		addFile(fileName.toStdString());
	}
}

void QTConsole::on_actionFileSave_triggered() {
	/*
	QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save VRJ Lua File..."),
            QString(),
            tr("VRJ Lua Files (*.vrjlua;*.lua);;All Files (*)"));

	if (!fileName.isEmpty() && !fileName.isNull()) {
		addFile(fileName.toStdString());
	}
	*/
}
		
void QTConsole::on_actionFileExit_triggered() {

}

void QTConsole::on_buttonRun_clicked() {
	std::string code = _ui->plainTextEdit->toPlainText().toStdString();
	_ui->plainTextEdit->setPlainText(QString());
	addString(code);
}

bool QTConsole::threadLoop() {
	if (_running) {
		/// @todo notify that the thread is already running?
		return false;
	}

	_running = true;

	show();
	_app->exec();

	_running = false;
	vrj::Kernel::instance()->stop();
	vrj::Kernel::instance()->waitForKernelStop();
	return true;
}

void QTConsole::stopThread() {
	//_running = false;
}

void QTConsole::appendToDisplay(std::string const& message) {
	QString text = _ui->plainTextEditLog->toPlainText();
	text.append(QString::fromStdString(message + "\n"));
	_ui->plainTextEditLog->setPlainText(text);
}

void QTConsole::setTitle(std::string const& title) {
	setWindowTitle(QString::fromStdString(title));
}

bool QTConsole::_doThreadWork() {
	//return ((Fl::wait(1.0/60.0) >= 0) && (Fl::first_window() != NULL));
	return true;
}

} // end of vrjLua namespace
