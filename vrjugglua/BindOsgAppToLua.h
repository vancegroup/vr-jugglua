/**	@file	BindOsgAppToLua.h
	@brief	header

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
#ifndef INCLUDED_vrjugglua_BindOsgAppToLua_h
#define INCLUDED_vrjugglua_BindOsgAppToLua_h

// Local includes
#include "LuaScript.h"
#include "bindUtils/WrapperClass_Macros.h"

// Library/third-party includes
#include <luabind/luabind.hpp>

#include <vrj/Draw/OSG/OsgApp.h>
#include <gadget/Type/PositionInterface.h>

#include <osg/ref_ptr>
#include <osg/Group>
#include <osgUtil/SceneView>


// Standard includes
// - none

namespace vrjLua {

class LuaOsgApp : public vrj::OsgApp {
	public:
		LuaOsgApp(/*vrj::Kernel* kern, int & argc, char** argv*/);
		virtual ~LuaOsgApp() {}
		virtual osg::Group* getScene();

		/// @name OsgApp interface
		/// @{
		virtual void init();
		virtual void initScene();

		virtual void configSceneView(osgUtil::SceneView* newSceneViewer);
		unsigned int getSceneViewDefaults();

		virtual void preFrame();
		virtual void latePreFrame();
		virtual void intraFrame();
		virtual void postFrame();
		/// @}

		/// @name Convenience functions for Lua apps
		/// @{
		double getTimeDelta();

		/// @}
	protected:
		osg::ref_ptr<osg::Group>           _root;
		vpr::Interval _lastPreFrameTime;
		double _timeDelta;
};

LuaOsgApp::LuaOsgApp(/*vrj::Kernel* kern, int & argc, char** argv*/) :
		vrj::OsgApp(vrj::Kernel::instance() /*kern , argc, argv */),
		_timeDelta(-1) {

}

inline osg::Group* LuaOsgApp::getScene() {
	return _root.get();
}

inline void LuaOsgApp::init() {
	_root = new osg::Group();
	vrj::OsgApp::init();
}

inline void LuaOsgApp::initScene() { ; }

inline void LuaOsgApp::configSceneView(osgUtil::SceneView * newSceneViewer) {
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
inline unsigned int LuaOsgApp::getSceneViewDefaults() {
	return osgUtil::SceneView::NO_SCENEVIEW_LIGHT;
}

inline void LuaOsgApp::preFrame() {
	vpr::Interval cur_time = vrj::Kernel::instance()->getUsers()[0]->getHeadUpdateTime();
	vpr::Interval diff_time(cur_time - _lastPreFrameTime);
	if (_lastPreFrameTime.getBaseVal() >= cur_time.getBaseVal()) {
		diff_time.secf(0.0f);
	}

	_timeDelta = diff_time.secf();
}
inline void LuaOsgApp::latePreFrame() {
	vrj::OsgApp::latePreFrame();
}

inline void LuaOsgApp::intraFrame() { ; }

inline void LuaOsgApp::postFrame() { ; }

inline double LuaOsgApp::getTimeDelta() { return _timeDelta; }


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


#endif // INCLUDED_vrjugglua_BindPositionInterfaceToLua_h
