/**	@file	Internal_PositionInterface.h
	@brief	PositionInterface wrapper class for LuaBind

	@date
	2009-2011

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

//          Copyright Iowa State University 2009-2011.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#pragma once
#ifndef INCLUDED_vrjugglua_Internal_PositionInterface_h
#define INCLUDED_vrjugglua_Internal_PositionInterface_h

// Internal Includes
// - none

// Library/third-party includes
#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/Position/PositionUnitConversion.h>

#include <gmtl/Matrix.h>

#include <osg/Matrixd>
#include <osg/Quat>
#include <osg/Vec3d>

// Standard includes
#include <string>

namespace vrjLua {
	namespace Internal {
		class PositionInterface {
			public:
				PositionInterface(const std::string & device);

				osg::Matrixd getMatrix();
				osg::Vec3d getPosition();
				osg::Quat getOrientation();
				osg::Vec3d getForwardVector();

			protected:
				gmtl::Matrix44f _getData(const float scale = gadget::PositionUnitConversion::ConvertToFeet);

				gadget::PositionInterface _iface;
		};
	} // end of Internal namespace
} // end of vrjLua namespace

#endif // INCLUDED_vrjugglua_Lua_PositionInterface_h
