/** @date	2010

        @author
        Ryan Pavlik ( <rpavlik@iastate.edu> http://academic.cleardefinition.com/
   ),
        Iowa State University
        Virtual Reality Applications Center and
        Human-Computer Interaction Graduate Program
*/

#define BOOST_TEST_MODULE OSG Numeric Conversion tests

#define BUILD_OSGLUABIND_VERBOSE

// Internal Includes
#include <luabind/scope.hpp>
#include <luabind/function.hpp>
#include <vrjugglua/osgLuaBind.h>
#include <vrjugglua/LuaScript.h>

// Library/third-party includes
#include <BoostTestTargetConfig.h>
#include <osgLua/introspection/Value>
#include <osgLua/introspection/Type>
#include <osg/Matrixd>
#include <osg/Vec3d>
#include <osg/ref_ptr>

// Standard includes
#include <iostream>
#include <iomanip>

#include "PathSetupDummy.h"

using namespace boost::unit_test;
using namespace vrjLua;
using namespace luabind;


struct Fixture {
    InitPath dummy; // Needed to make LuaScript start OK.
    LuaScript s; // Needed to initialize the extra converters
};


BOOST_AUTO_TEST_CASE(ConversionPathVec3fToVec3d) {
    osgLua::introspection::Value v = osg::Vec3f(1, 1, 1);
    BOOST_CHECK(!v.tryConvertTo(typeof(osg::Vec3d)).isEmpty());
}

BOOST_AUTO_TEST_CASE(ConstructionVec3fToVec3d) {
    osgLua::introspection::Value v = osg::Vec3f(1, 1, 1);
    osgLua::introspection::ValueList args;
    args.push_back(v);
    BOOST_CHECK_NO_THROW(typeof(osg::Vec3d).createInstance(args));
}

BOOST_AUTO_TEST_CASE(Vec3fToVec3dCompatibleSet) {
    osgLua::introspection::Value vf = osg::Vec3f(1, 1, 1);
    osgLua::introspection::ValueList args;
    args.push_back(vf);

    BOOST_CHECK(typeof(osg::Vec3d).getCompatibleMethod("set", args, true));
}

BOOST_AUTO_TEST_CASE(Vec3fToVec3dUsingSet) {
    osgLua::introspection::Value vf = osg::Vec3f(1, 1, 1);
    osgLua::introspection::ValueList args;
    args.push_back(vf);

    osgLua::introspection::Value vd = osg::Vec3d();
    BOOST_CHECK_NO_THROW(
        typeof(osg::Vec3d).invokeMethod("set", vd, args, true));
}

BOOST_AUTO_TEST_CASE(GetConversionVec3fToVec3d) {
    osgLua::introspection::Value vf = osg::Vec3f(1, 1, 1);
    osgLua::introspection::ValueList args;
    args.push_back(vf);

    osgLua::introspection::Value vd = osg::Vec3d();
    BOOST_CHECK(osgLua::introspection::Reflection::getConverter(
        typeof(osg::Vec3f), typeof(osg::Vec3d)));
    BOOST_CHECK(osgLua::introspection::Reflection::getConverter(
        typeof(osg::Vec3d), typeof(osg::Vec3f)));
}
