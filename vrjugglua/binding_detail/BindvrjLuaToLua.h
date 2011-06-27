/**	@file	binding_detail/BindvrjLuaToLua.h
	@brief	header exposing binding function for vrjLua convenience functions

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
#ifndef INCLUDED_vrjugglua_binding_detail_BindvrjLuaToLua_h
#define INCLUDED_vrjugglua_binding_detail_BindvrjLuaToLua_h

// Local includes
#include <vrjugglua/LuaScript.h>

// Library/third-party includes
// - none

// Standard includes
// - none

namespace vrjLua {

	void BindvrjLuaToLua(LuaStatePtr state);

} // end of vrjLua namespace



#endif // INCLUDED_vrjugglua_binding_detail_BindvrjLuaToLua_h
