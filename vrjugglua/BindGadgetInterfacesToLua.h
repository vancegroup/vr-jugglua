/**	@file	BindPositionInterfaceToLua.h
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
#ifndef INCLUDED_vrjugglua_BindGadgetInterfacesToLua_h
#define INCLUDED_vrjugglua_BindGadgetInterfacesToLua_h

// Local includes
#include "LuaScript.h"

// Library/third-party includes
// - none

// Standard includes
// - none

namespace vrjLua {

	void bindGadgetInterfacesToLua(LuaStatePtr state);

}// end of vrjLua namespace

#endif // INCLUDED_vrjugglua_BindGadgetInterfacesToLua_h
