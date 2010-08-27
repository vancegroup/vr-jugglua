/**	@file	Internal_PositionInterface.h
	@brief	PositionInterface wrapper class for LuaBind

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
#ifndef INCLUDED_vrjugglua_Internal_PositionInterface_h
#define INCLUDED_vrjugglua_Internal_PositionInterface_h

// Internal Includes
#include "LuaScript.h"

// Library/third-party includes
#include <gadget/Type/PositionInterface.h>

#include <gmtl/Matrix.h>

#include <osg/Matrix>
#include <osg/Vec3>

// Standard includes
// - none

namespace vrjLua {
namespace Internal {
	class PositionInterface {
		public:
			PositionInterface(const std::string & device);

			osg::Matrixd getMatrix();
			osg::Vec3d getPosition();
			osg::Vec4d getOrientation();
			double getQuatX();
			double getQuatY();
			double getQuatZ();
			double getQuatW();
			osg::Vec3d getForwardVector();

		protected:
			gmtl::Matrix44f _getData(const float scale = gadget::PositionUnitConversion::ConvertToFeet);

			gadget::PositionInterface _iface;
	};
} // end of Internal namespace
} // end of vrjLua namespace

#endif // INCLUDED_vrjugglua_Lua_PositionInterface_h
