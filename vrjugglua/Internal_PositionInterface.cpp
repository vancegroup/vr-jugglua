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

#include <util/gmtlToOsgMatrix.h>

// Library/third-party includes
#include <gadget/Type/Position/PositionUnitConversion.h>
#include <gmtl/Generate.h>
#include <gmtl/Matrix.h>                // for Matrix, Matrix44f
#include <gmtl/Vec.h>                   // for Vec3d, Vec4f
#include <gmtl/Xforms.h>                // for xform
#include <osg/Matrix>                   // for Matrix
#include <osg/Matrixd>                  // for Matrixd
#include <osg/Quat>                     // for Quat
#include <osg/Vec3d>                    // for Vec3d

// Standard includes
// - none

namespace vrjLua {
	namespace Internal {
		PositionInterface::PositionInterface(const std::string & device) {
			_iface.init(device);
		}

		inline gmtl::Matrix44f PositionInterface::_getData(const float scale) {
			return _iface->getData(scale);
		}

		osg::Matrixd PositionInterface::getMatrix() {
			return util::toOsgMatrix(_iface->getData(gadget::PositionUnitConversion::ConvertToMeters));
		}

		osg::Vec3d PositionInterface::getPosition() {
			gmtl::Vec3d xlate = gmtl::makeTrans<gmtl::Vec3d>(_iface->getData(gadget::PositionUnitConversion::ConvertToMeters));
			return osg::Vec3d(xlate[0], xlate[1], xlate[2]);
		}

		osg::Quat PositionInterface::getOrientation() {
			return util::toOsgMatrix(_iface->getData(gadget::PositionUnitConversion::ConvertToMeters)).getRotate();
		}

		osg::Vec3d PositionInterface::getForwardVector() {
			gmtl::Vec4f fwd;
			gmtl::xform(fwd, _iface->getData(gadget::PositionUnitConversion::ConvertToMeters), gmtl::Vec4f(0, 0, -1, 0));
			return osg::Vec3d(fwd[0], fwd[1], fwd[2]);
		}
	} // end of Internal namespace
} // end of vrjLua namespace
