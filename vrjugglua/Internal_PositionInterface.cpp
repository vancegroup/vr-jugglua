/**	@file	Internal_PositionInterface.cpp
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
#include "Internal_PositionInterface.h"

#include <util/osg/gmtlToOsgMatrix.h>

// Library/third-party includes
#include <gmtl/Generate.h>

// Standard includes
// - none

namespace vrjLua {
namespace Internal {
PositionInterface::PositionInterface() :
	_ready(false) {
}

void PositionInterface::init(const std::string & device) {
	_iface.init(device);
	_ready = true;
}

inline gmtl::Matrix44f PositionInterface::_getData(const float scale) {
	if (_ready) {
		/// @todo Error condition if not initialized
	}
	return _iface->getData(scale);
}

osg::Matrixf PositionInterface::getMatrix() {
	return util::osg::toOsgMatrix(_iface->getData(gadget::PositionUnitConversion::ConvertToMeters));
}

osg::Vec3f PositionInterface::getPosition() {
	gmtl::Vec3f xlate = gmtl::makeTrans<gmtl::Vec3f>(_iface->getData(gadget::PositionUnitConversion::ConvertToMeters));
	return osg::Vec3f(xlate[0], xlate[1], xlate[2]);
}

osg::Vec3f PositionInterface::getForwardVector() {
	return osg::Vec3f(0,0,-1) * util::osg::toOsgMatrix(_iface->getData(gadget::PositionUnitConversion::ConvertToMeters));
}
} // end of Internal namespace
} // end of vrjLua namespace
