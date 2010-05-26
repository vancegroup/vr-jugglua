/**	@file	BindOsgAppToLua.cpp
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

// Local includes
#include "BindOsgAppToLua.h"

// Library/third-party includes
#include <osg/ref_ptr>
#include <osg/Group>
#include <osgUtil/SceneView>

// Standard includes
#ifdef VERBOSE
#include <iostream>
#endif

namespace vrjLua {

osg::Group* LuaOsgApp::getScene() {
	return _root.get();
}

void LuaOsgApp::init() {
	_root = new osg::Group();
	vrj::OsgApp::init();
}

void LuaOsgApp::initScene() { ; }

unsigned int LuaOsgApp::getSceneViewDefaults() {
	return osgUtil::SceneView::NO_SCENEVIEW_LIGHT;
}

void LuaOsgApp::latePreFrame() {
	vrj::OsgApp::latePreFrame();
}

void LuaOsgApp::intraFrame() { ; }

void LuaOsgApp::postFrame() { ; }

double LuaOsgApp::getTimeDelta() { return _timeDelta; }

LuaOsgApp::LuaOsgApp(/*vrj::Kernel* kern, int & argc, char** argv*/) :
		vrj::OsgApp(vrj::Kernel::instance() /*kern , argc, argv */),
		_timeDelta(-1) {

}

void LuaOsgApp::configSceneView(osgUtil::SceneView * newSceneViewer) {
	vrj::OsgApp::configSceneView(newSceneViewer);
	osg::ref_ptr<osg::Light> light0;
	osg::ref_ptr<osg::LightSource> lightSource0;
	light0 = new osg::Light();
	light0->setLightNum(0);
	light0->setAmbient(osg::Vec4f(0.36862f, 0.36842f, 0.36842f, 1.0f));
	light0->setDiffuse(osg::Vec4f(0.88627f, 0.88500f, 0.88500f, 1.0f));
	light0->setSpecular(osg::Vec4f(0.49019f, 0.48872f, 0.48872f, 1.0f));
	//light0->setPosition(osg::Vec4f(0.0f, 10000.0f, -20.0f, 0.0f));
	light0->setPosition(osg::Vec4f(10000.0f, 10000.0f, 10000.0f, 0.0f));
	light0->setDirection(osg::Vec3f(-1.0f, -1.0f, -1.0f));

	lightSource0 = new osg::LightSource();
	lightSource0->setLight(light0.get());
	lightSource0->setLocalStateSetModes(osg::StateAttribute::ON);

	// Now that we know we have a root node add the default light to the
	// scene.
	this->getScene()->addChild( lightSource0.get() );
}

void LuaOsgApp::preFrame() {
	vpr::Interval cur_time = vrj::Kernel::instance()->getUsers()[0]->getHeadUpdateTime();
	vpr::Interval diff_time(cur_time - _lastPreFrameTime);
	if (_lastPreFrameTime.getBaseVal() >= cur_time.getBaseVal()) {
		diff_time.secf(0.0f);
	}

	_timeDelta = diff_time.secf();
}


#define BASECLASS LuaOsgApp
#define WRAPPERCLASS LuaOsgApp_wrapper

/// @brief Wrapper class to permit inheritance in Lua
BEGIN_WRAPPER_CLASS {
	public:
	WRAPPERCLASS() {}

	METHOD_WRAP(void, init, (BASECLASS::init()), );

	METHOD_WRAP(void, initScene, , );

	METHOD_WRAP(void, preFrame, (BASECLASS::preFrame()), );

	METHOD_WRAP(void, latePreFrame, , (BASECLASS::latePreFrame()));

	METHOD_WRAP(void, intraFrame, , );

	METHOD_WRAP(void, postFrame, , );

	METHOD_WRAP(double, getTimeDelta, , );
};

void bindOsgAppToLua(LuaStatePtr state) {
#ifdef VERBOSE
	std::cerr << "Registering vrj.OsgApp with Lua..." << std::flush << std::endl;
#endif
	using namespace luabind;
	module(state.get(), "vrj") [
		class_<BASECLASS, WRAPPERCLASS>("OsgApp")
			.def(constructor<>())
			METHOD_BIND(init)
			METHOD_BIND(initScene)
			METHOD_BIND(preFrame)
			METHOD_BIND(latePreFrame)
			METHOD_BIND(intraFrame)
			METHOD_BIND(postFrame)
			METHOD_BIND(getTimeDelta)
	];
}


#undef BASECLASS
#undef STATIC_METHOD
}// end of vrjLua namespace
