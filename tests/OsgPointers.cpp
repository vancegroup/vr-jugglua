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

struct Fixture {
	Fixture() {
		LuaStatePtr ptr(s.getLuaState().lock());
		module(ptr.get())[
			def("nodeFunc", &nodeFunc)
		];
	}
	LuaScript s;

};

BOOST_AUTO_TEST_CASE(ExactMatch) {
	Fixture f;
	BOOST_CHECK(f.s.runString("node = osg.Node(); nodeFunc(node)"));
}


BOOST_AUTO_TEST_CASE(SingleConversion) {
	Fixture f;
	BOOST_CHECK(f.s.runString("node = osg.Group(); nodeFunc(node)"));
}

BOOST_AUTO_TEST_CASE(MultipleConversion) {
	Fixture f;
	BOOST_CHECK(f.s.runString("node = osg.PositionAttitudeTransform(); nodeFunc(node)"));
}

