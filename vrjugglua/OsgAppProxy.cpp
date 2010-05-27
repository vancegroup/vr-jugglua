/**	@file	OsgAppProxy.cpp
	@brief Application proxy object implementation

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
#include "OsgAppProxy.h"

// Library/third-party includes
#include <luabind/class.hpp>

// Standard includes
#include <cstring>

namespace vrjLua {
/// Initialize static member variable
OsgAppProxy* OsgAppProxy::_pApp = NULL;

OsgAppProxy::OsgAppProxy() :
	vrj::OsgApp(vrj::Kernel::instance()),
	_timeDelta(-1) {

}

OsgAppProxy::OsgAppProxy(vrj::Kernel* kern/*, int & argc, char** argv*/) :
	vrj::OsgApp(kern),
	_timeDelta(-1)
{
	/// update static pointer to app object
	_pApp = this;
}

void OsgAppProxy::bindToLua(LuaStatePtr & state) {
	// Bind this class
	if (state) {
		std::cerr << "Binding the OsgAppProxy to Lua" << std::endl;
		// we lock the pointer here to borrow it
		using namespace luabind;
		module(state.get(), "vrjApp") [
			class_<OsgAppProxy>("OsgAppProxy")
				.def(constructor<>())
				.def("setAppDelegate", & OsgAppProxy::setAppDelegate)
				.def("getAppDelegate", & OsgAppProxy::getAppDelegate)
				.def("setActiveApplication", & OsgAppProxy::setActiveApplication)
				.def("getScene", & OsgAppProxy::getScene)
				.def("getTimeDelta", & OsgAppProxy::getTimeDelta)
	   ];
	}
}

OsgAppProxy::~OsgAppProxy() {

}

void OsgAppProxy::setActiveApplication() {
	vrj::Kernel::instance()->setApplication(this);
}


void OsgAppProxy::setAppDelegate(luabind::object const & delegate) {
	/// @todo test here to see if the passed delegate is suitable
	std::cerr << "Trying to set the app delegate..." << std::endl;
	if (luabind::type(delegate) == LUA_TTABLE) {
		_delegate = delegate;
	} else {
		std::cerr << "Lua app tried to set an invalid app delegate!" << std::endl;
	}
}

luabind::object const & OsgAppProxy::getAppDelegate() {
	return _delegate;
}

void OsgAppProxy::initScene() {
	std::cout << "---------- OsgAppProxy::initScene() ---------------" << std::endl;

	// Create the top level node of the tree
	_rootNode = new osg::Group();

	/// @todo call delegate here
}

void OsgAppProxy::configSceneView(osgUtil::SceneView* newSceneViewer) {
	vrj::OsgApp::configSceneView(newSceneViewer);
/*
	newSceneViewer->getLight()->setAmbient(osg::Vec4(0.5f, 0.5f, 0.5f, 1.0f));
	newSceneViewer->getLight()->setDiffuse(osg::Vec4(0.9f, 0.9f, 0.9f, 1.0f));
	newSceneViewer->getLight()->setSpecular(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	*/
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
	getScene()->addChild( lightSource0.get() );
}

void OsgAppProxy::preFrame() {
	vpr::Interval cur_time = vrj::Kernel::instance()->getUsers()[0]->getHeadUpdateTime();
	vpr::Interval diff_time(cur_time - _lastPreFrameTime);
	if (_lastPreFrameTime.getBaseVal() >= cur_time.getBaseVal()) {
		diff_time.secf(0.0f);
	}

	_timeDelta = diff_time.secf();

	/// @todo call delegate here

	_lastPreFrameTime = cur_time;
}

void OsgAppProxy::latePreFrame() {
	/// @todo call delegate here

	// Finish updating the scene graph.
	vrj::OsgApp::latePreFrame();
}

void OsgAppProxy::intraFrame() {
	/// @todo call delegate here
}

void OsgAppProxy::postFrame() {
	/// @todo call delegate here
}

} // end of vrjLua namespace
