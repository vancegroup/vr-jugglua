/** @date	2011

	@author
	Ryan Pavlik ( <rpavlik@iastate.edu> http://academic.cleardefinition.com/ ),
	Iowa State University
	Virtual Reality Applications Center and
	Human-Computer Interaction Graduate Program
*/

#define BOOST_TEST_MODULE Set Property With OSG Type tests

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
#include <osg/ref_ptr>

// Standard includes
#include <iostream>
#include <iomanip>

using namespace boost::unit_test;
using namespace vrjLua;
using namespace luabind;

//To run the tests:
//change directory to build\vr-jugglua
//ctest -C RelWithDebInfo -R SetPropertyWithOsgType --output-on-failure
//or for verbose output for all
//ctest -C RelWithDebInfo -R SetPropertyWithOsgType -V

//define how to output a matrix (this will be used later by BOOST)
namespace osg {
	std::ostream & operator<<(std::ostream & os, Matrixd const m) {
		for (unsigned int row = 0; row < 4; ++row) {
			for (unsigned int col = 0; col < 4; ++col) {
				os << std::setw(10) << m(row, col);
				if (col != 3) {
					os << ", ";
				}
			}
			os << std::endl;
		}
		return os;
	}
}

void passconstbyref(osg::Matrixd const &x) {
	std::cout << "In passconstbyref call test." << std::endl;
	BOOST_CHECK_EQUAL(x, osg::Matrixd::identity());
}
void passconst(osg::Matrixd const x) {
	std::cout << "In passconst call test." << std::endl;
	BOOST_CHECK_EQUAL(x, osg::Matrixd::identity());
}

class A {
	private:
		osg::Matrixd value;
	public:
		A(){}; //constructor
		void set(osg::Matrixd const &x){
			value = x;
		};
		osg::Matrixd get() const {
			return value;
		};
};

struct Fixture {
	Fixture() {
		LuaStatePtr ptr(s.getLuaState());
		module(ptr.get())[
			def("passconstbyref", &passconstbyref),
			def("passconst", &passconst),
			class_<A>("A")
			.def(constructor<>())
			.property("val", &A::get, &A::set)
		];
	}
	LuaScript s;
};


	
BOOST_AUTO_TEST_CASE(TestMatrixAssignment) {
	Fixture f;
	BOOST_CHECK(f.s.runString("A().val = osg.Matrixd.identity()"));
}

BOOST_AUTO_TEST_CASE(TestPassConstByRef) {
	Fixture f;
	BOOST_CHECK(f.s.runString("passconstbyref(osg.Matrixd.identity())"));
}

BOOST_AUTO_TEST_CASE(TestPassConst) {
	Fixture f;
	BOOST_CHECK(f.s.runString("passconst(osg.Matrixd.identity())"));
}
