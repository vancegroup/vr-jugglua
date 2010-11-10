/** @date	2010

	@author
	Ryan Pavlik ( <rpavlik@iastate.edu> http://academic.cleardefinition.com/ ),
	Iowa State University
	Virtual Reality Applications Center and
	Human-Computer Interaction Graduate Program
*/

#define BOOST_TEST_MODULE OsgPointers tests

// Internal Includes
#include <vrjugglua/osgLuaBind.h>
#include <vrjugglua/LuaScript.h>

// Library/third-party includes
#include <BoostTestTargetConfig.h>
#include <osg/Node>

// Standard includes
#include <iostream>


using namespace boost::unit_test;
using namespace vrjLua;
using namespace luabind;

void nodeFunc(osg::Node * node) {
	std::cout << "Got the pointer " << node << std::endl;
}

void vecFunc(osg::Vec3d vec) {
	std::cout << "Got the vector " << vec.x() << "," << vec.y() << "," << vec.z() << std::endl;
}

struct Fixture {
	Fixture() {
		LuaStatePtr ptr(s.getLuaState().lock());
		module(ptr.get())[
			def("nodeFunc", &nodeFunc),
			def("vecFunc", &vecFunc)
		];
	}
	LuaScript s;

};

BOOST_AUTO_TEST_CASE(RefExactMatch) {
	Fixture f;
	BOOST_CHECK(f.s.runString("node = osg.Node(); nodeFunc(node)"));
}


BOOST_AUTO_TEST_CASE(RefSingleConversion) {
	Fixture f;
	BOOST_CHECK(f.s.runString("node = osg.Group(); nodeFunc(node)"));
}

BOOST_AUTO_TEST_CASE(RefMultipleConversion) {
	Fixture f;
	BOOST_CHECK(f.s.runString("node = osg.PositionAttitudeTransform(); nodeFunc(node)"));
}

BOOST_AUTO_TEST_CASE(RefInvalidConversion) {
	Fixture f;
	BOOST_CHECK(!f.s.runString("node = osg.Matrix(); nodeFunc(node)"));
}

BOOST_AUTO_TEST_CASE(RefInvalidRefConversion) {
	Fixture f;
	BOOST_CHECK(!f.s.runString("node = osg.RefMatrix(); nodeFunc(node)"));
}


BOOST_AUTO_TEST_CASE(ValExactMatch) {
	Fixture f;
	BOOST_CHECK(f.s.runString("vec = osg.Vec3d(); vecFunc(vec)"));
}

BOOST_AUTO_TEST_CASE(ValSingleConversion) {
	Fixture f;
	BOOST_CHECK(f.s.runString("vec = osg.Vec3(); vecFunc(vec)"));
}

BOOST_AUTO_TEST_CASE(ValSingleConversionFromFloat) {
	Fixture f;
	BOOST_CHECK(f.s.runString("vec = osg.Vec3f(); vecFunc(vec)"));
}

BOOST_AUTO_TEST_CASE(ValInvalidConversion) {
	Fixture f;
	BOOST_CHECK(!f.s.runString("vec = osg.Node(); vecFunc(vec)"));
}

BOOST_AUTO_TEST_CASE(ValInvalidValConversion) {
	Fixture f;
	BOOST_CHECK(!f.s.runString("vec = osg.Matrix(); vecFunc(vec)"));
}

