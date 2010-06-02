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

// Standard includes
#include <boost/shared_ptr.hpp>

using boost::shared_ptr;

namespace vrjLua {

FLTKNav::FLTKNav() :
			NavTestbedUI(480, 560) {
	_input->buffer(_inputBuf);
	_existingCode->buffer(_codeBuf);
}

FLTKNav::~FLTKNav() {

}

void FLTKNav::run() {

}

void FLTKNav::runInput() {
	shared_ptr<const char> input_chars(_inputBuf.text(), &free);
	std::string input(input_chars.get());
	bool ret = _nav.runString(input);
	if (ret) {
		// Successful - append to the text display

	}
}

void FLTKNav::chooseFile() {

}

} // end of vrjLua namespace
