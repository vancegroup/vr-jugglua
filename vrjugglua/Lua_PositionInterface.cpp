/**	@file	Lua_PositionInterface.cpp
	@brief	LuaBind code for the PositionInterface

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
#include "LuaScript.h"
#include "Lua_PositionInterface.h"

#ifndef LUABIND_COMBINED_COMPILE
#include "BindPositionInterfaceToLua.h"
#endif

#include <util/osg/gmtlToOsgMatrix.h>

// Library/third-party includes


#include <gmtl/Generate.h>

// Standard includes
// - none

namespace vrjLua {
PositionInterface::PositionInterface() :
	_ready(false) {
}

inline void PositionInterface::init(const std::string & device) {
	_iface.init(device);
	_ready = true;
}

inline gmtl::Matrix44f PositionInterface::_getData(const float scale) {
	if (_ready) {
		/// @todo Error condition if not initialized
	}
	return _iface->getData(scale);
}

inline osg::Matrixf PositionInterface::getMatrix() {
	return util::osg::toOsgMatrix(_iface->getData(gadget::PositionUnitConversion::ConvertToMeters));
}

inline osg::Vec3f PositionInterface::getPosition() {
	gmtl::Vec3f xlate = gmtl::makeTrans<gmtl::Vec3f>(_iface->getData(gadget::PositionUnitConversion::ConvertToMeters));
	return osg::Vec3f(xlate[0], xlate[1], xlate[2]);
}

inline osg::Vec3f PositionInterface::getForwardVector() {
	return osg::Vec3f(0,0,-1) * util::osg::toOsgMatrix(_iface->getData(gadget::PositionUnitConversion::ConvertToMeters));
}

} // end of vrjLua namespace
