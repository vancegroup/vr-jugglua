/**	@file	NavInteractive.h
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
#ifndef INCLUDED_src_NavInteractive_h
#define INCLUDED_src_NavInteractive_h

// Local includes
#include <vrjugglua/VRJLua_C_Interface.h>
#include <vrjugglua/LuaScript.h>
#include <vrjugglua/SynchronizedRunBuffer.h>

// Library/third-party includes
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

// Standard includes
#include <string>

namespace vrjLua {

class NavInteractive {
	public:
		NavInteractive();
		~NavInteractive();
		bool runFile(const std::string & fn);
		bool runString(const std::string & str);

	protected:
		LuaScript _script;
		SynchronizedRunBuffer* _runbuf;
};

// -- inline implementations -- /

}// end of vrjLua namespace

#endif // INCLUDED_src_NavInteractive_h
