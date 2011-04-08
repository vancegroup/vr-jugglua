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
#include "FLTKConsole.h"
#include "../VRJLuaOutput.h"

// FLTK-generated source
#include <vrjlua-fltk-console.h>

// Library/third-party includes
#include <boost/scoped_ptr.hpp>

#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Native_File_Chooser.H>

#include <vrj/Kernel/Kernel.h>

// Standard includes
#include <iostream>
#include <stdexcept>


namespace vrjLua {

void doNothingUponModify(int, int, int, int, const char *, void *) {
	// Do nothing
	// FLTK whines at shutdown if there's no modify callback for its text buffers
}

void doNothingUponPreDelete(int, int, void*) {
	// Do nothing
	// FLTK whines at shutdown if there's no predelete callback for its text buffers
}


class FLTKConsoleView : public FLTKConsoleUI {
	public:
		FLTKConsoleView(FLTKConsole* console) :
				_console(console),
				FLTKConsoleUI(700, 590, "VRJLua Console"),
				_inputBuf(new Fl_Text_Buffer()),
				_codeBuf(new Fl_Text_Buffer()) {
			_input->buffer(_inputBuf.get());
			_existingCode->buffer(_codeBuf.get());

			_inputBuf->add_modify_callback(doNothingUponModify, NULL);
			_codeBuf->add_modify_callback(doNothingUponModify, NULL);

			_inputBuf->add_predelete_callback(doNothingUponPreDelete, NULL);
			_codeBuf->add_predelete_callback(doNothingUponPreDelete, NULL);

			show();
		}

		virtual void runInput() {
			std::string input;
			{
				char * input_chars = _inputBuf->text();
				input = std::string(input_chars);
				free(input_chars);
			}

			bool ret = _console->addString(input);
			if (ret) {
				_inputBuf->text("");
			}
		}


		virtual void chooseAndRunFile() {
			boost::shared_ptr<Fl_Native_File_Chooser> fc(new Fl_Native_File_Chooser(Fl_Native_File_Chooser::BROWSE_FILE));
			fc->title("Open VRJLua File...");
			fc->filter("VRJLua files\t*.{vrjlua,lua}");

			int ret = fc->show();
			if (ret != 0) {
				// No file picked, for one reason or another.
				return;
			}

			appendToDisplay("\n-- Run file chosen in GUI");
			_console->addFile(fc->filename());
		}

		virtual void saveFile() {
			Fl_Native_File_Chooser fc(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
			fc.title("Save VRJLua File...");
			fc.filter("VRJLua files\t*.{vrjlua,lua}");

			int ret = fc.show();
			if (ret != 0) {
				// No file picked, for one reason or another.
				return;
			}

			int saveret = _codeBuf->savefile(fc.filename());
			if (saveret == 0) {
				// Successful - append to the text display
				std::string code("-- code up to here saved to file '");
				code += fc.filename();
				code += "'\n";
				appendToDisplay(code.c_str());
			} else {
				appendToDisplay("\n-- Saving to chosen file failed");
			}
		}

		virtual void appendToDisplay(std::string const& message) {
			_codeBuf->append(message.c_str());
			_codeBuf->append("\n");
			// Scroll to bottom
			_existingCode->scroll(_existingCode->count_lines(0, _codeBuf->length(), 1), 0);
		}

	protected:
		FLTKConsole * _console;
		boost::scoped_ptr<Fl_Text_Buffer> _inputBuf;
		boost::scoped_ptr<Fl_Text_Buffer> _codeBuf;
};

void FLTKConsole::setup(int & argc, char * argv[]) {
	Fl::args(argc, argv);
	Fl::get_system_colors();
}

FLTKConsole::FLTKConsole() :
		LuaConsole(),
		_running(false) {
#ifdef VERBOSE
	std::cout << "In constructor " << __FUNCTION__ << " at " << __FILE__ << ":" << __LINE__ << " with this=" << this << std::endl;
#endif
	_view = boost::shared_ptr<FLTKConsoleView>(new FLTKConsoleView(this));
}

FLTKConsole::FLTKConsole(LuaScript const& script) :
		LuaConsole(script),
		_running(false){
#ifdef VERBOSE
	std::cout << "In constructor " << __FUNCTION__ << " at " << __FILE__ << ":" << __LINE__ << " with this=" << this << std::endl;
#endif
	_view = boost::shared_ptr<FLTKConsoleView>(new FLTKConsoleView(this));
}

FLTKConsole::~FLTKConsole() {
#ifdef VERBOSE
	std::cout << "In destructor " << __FUNCTION__ << " at " << __FILE__ << ":" << __LINE__ << " with this=" << this << std::endl;
#endif
	stopThread();
}

bool FLTKConsole::threadLoop() {
	if (_running) {
		/// @todo notify that the thread is already running?
		return false;
	}

	_running = true;
	bool ret = true;
	while (_running && vrj::Kernel::instance()->isRunning()) {
		// Do the FLTK loop
		ret = _doThreadWork();
		if (!ret) {
			// Exit originating from FLTK
			break;
		}
	}
	_running = false;
	vrj::Kernel::instance()->stop();
	vrj::Kernel::instance()->waitForKernelStop();
	return true;
}

void FLTKConsole::stopThread() {
	_running = false;
}

void FLTKConsole::appendToDisplay(std::string const& message) {
	_view->appendToDisplay(message);
}

void FLTKConsole::setTitle(std::string const& title) {
	_view->copy_label(title.c_str());
}

void FLTKConsole::disableAction() {
	VRJLUA_MSG_START(dbgVRJLUA_CONSOLE, MSG_STATUS)
				<< "FLTKConsole::disableAction called but not implemented!!!"
				<< VRJLUA_MSG_END(dbgVRJLUA_CONSOLE, MSG_STATUS);
}

bool FLTKConsole::_doThreadWork() {
	return ((Fl::wait(1.0/60.0) >= 0) && (Fl::first_window() != NULL));
}

} // end of vrjLua namespace
