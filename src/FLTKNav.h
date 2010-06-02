/**	@file	FLTKNav.h
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
#ifndef INCLUDED_src_FLTKNav_h
#define INCLUDED_src_FLTKNav_h

// Local includes
#include <NavTestbedUI.h>
#include "NavInteractive.h"

// Library/third-party includes
#include <FL/Fl_Text_Buffer.H>


// Standard includes
// - none

namespace vrjLua {

class FLTKNav : public NavTestbedUI {
	public:
		FLTKNav();
		virtual ~FLTKNav();

		void run();

		/// @name Interface required by NavTestbedUI
		/// @{
		virtual void runInput();
		virtual void chooseFile();
		/// @}

	protected:
		NavInteractive _nav;
		Fl_Text_Buffer _inputBuf;
		Fl_Text_Buffer _codeBuf;

};

// -- inline implementations -- /

}// end of vrjLua namespace

#endif // INCLUDED_src_FLTKNav_h
