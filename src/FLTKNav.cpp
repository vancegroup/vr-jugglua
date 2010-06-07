/**	@file	FLTKNav
	@brief	implementation

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
#include "FLTKNav.h"

// Library/third-party includes
#include <FL/Fl_Native_File_Chooser.H>

#include <vrj/Kernel/Kernel.h>

// Standard includes
// - none

namespace vrjLua {

FLTKNav::FLTKNav() :
			NavTestbedUI(480, 560),
			_inputBuf(new Fl_Text_Buffer()),
			_codeBuf(new Fl_Text_Buffer()) {
	_input->buffer(_inputBuf.get());
	_existingCode->buffer(_codeBuf.get());

	/// Add a line showing the effective result of running the initial code
	_codeBuf->append("navtransform = osg.Group()\n");
}

FLTKNav::~FLTKNav() {

}

int FLTKNav::run() {
	show();
	/// Event loop: run FLTK events, until either FLTK or VRJ exits
	do {
	} while ((Fl::wait(0) >= 0) && (Fl::first_window() != NULL) && vrj::Kernel::instance()->isRunning());

	/// Shut down kernel nicely
	if (vrj::Kernel::instance()->isRunning()) {
		vrj::Kernel::instance()->stop();
	}
	return 0;
}

void FLTKNav::runInput() {
	std::string input;
	{
		char * input_chars = _inputBuf->text();
		input = std::string(input_chars);
		free(input_chars);
	}
	bool ret = _nav.runString(input);
	if (ret) {
		// Successful - append to the text display
		_codeBuf->append("\n");
		_codeBuf->append(input.c_str());
		_inputBuf->text("");
	} else {
		_codeBuf->append("\nExecution of input failed - check for errors and try again");
	}

	// Scroll to bottom
	_existingCode->scroll(_existingCode->count_lines(0, _codeBuf->length(), 1), 0);
}

void FLTKNav::chooseFile() {

	_fc = boost::shared_ptr<Fl_Native_File_Chooser>(new Fl_Native_File_Chooser(Fl_Native_File_Chooser::BROWSE_FILE));
	_fc->title("Open VRJLua File...");
	_fc->filter("Lua files\t*.lua");

	int ret = _fc->show();
	if (ret != 0) {
		// No file picked, for one reason or another.
		return;
	}

	bool bret = _nav.runFile(_fc->filename());
	if (bret) {
		// Successful - append to the text display
		std::string code("require('");
		code += _fc->filename();
		code += "')";
		_codeBuf->append("\n-- Run file chosen in GUI\n");
		_codeBuf->append(code.c_str());
	} else {
		_codeBuf->append("\nExecution of chosen file failed - check for errors and try again");
	}
}

} // end of vrjLua namespace
