/**	@file	Internal_DigitalInterface.h
	@brief	DigitalInterface wrapper class for LuaBind

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
#ifndef INCLUDED_vrjugglua_Internal_DigitalInterface_h
#define INCLUDED_vrjugglua_Internal_DigitalInterface_h

// Internal Includes
#include "LuaScript.h"

// Library/third-party includes
#include <gadget/Type/DigitalInterface.h>

#include <gmtl/Matrix.h>

#include <osg/Matrix>
#include <osg/Vec3f>

// Standard includes
// - none

namespace vrjLua {
namespace Internal {
	class DigitalInterface {
		public:
			DigitalInterface();

			void init(const std::string & device);

			bool isPressed();
			bool isAChange();

			bool wasJustPressed();
			bool wasJustReleased();

		protected:
			bool _ready;
			gadget::DigitalInterface _iface;
	};
} // end of Internal namespace
} // end of vrjLua namespace

#endif // INCLUDED_vrjugglua_Internal_DigitalInterface_h
