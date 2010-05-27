/**	@file	LuaOSGApp.cpp
	@brief Application object implementation

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
#include "LuaOSGApp.h"

// Library/third-party includes
#include <luabind/class.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

// Standard includes
#include <cstring>

/// Initialize static member variable
VRApp* VRApp::m_pApp = NULL;

VRApp::VRApp(vrj::Kernel* kern/*, int & argc, char** argv*/) :
	vrj::OsgApp(kern),
	m_timeDelta(-1)
{
	/// update static pointer to app object
	m_pApp = this;

	// Bind this class
	vrjLua::LuaStatePtr p = _luaScript.getLuaState().lock();
	if (p) {
		std::cerr << "Binding the OsgAppProxy to Lua" << std::endl;
		// we lock the pointer here to borrow it
		using namespace luabind;
		module(p.get(), "vrjApp") [
		   class_<VRApp>("OsgAppProxy")
		   .def("setAppDelegate", & VRApp::setAppDelegate)
		   .def("getAppDelegate", & VRApp::getAppDelegate)
	   ];
	}
}

VRApp::~VRApp() {

}

void VRApp::setAppDelegate(luabind::object const & delegate) {
	/// @todo test here to see if the passed delegate is suitable
	std::cerr << "Trying to set the app delegate..." << std::endl;
	if (luabind::type(delegate) == LUA_TTABLE) {
		_delegate = delegate;
	} else {
		std::cerr << "Lua app tried to set an invalid app delegate!" << std::endl;
	}
}

luabind::object const & VRApp::getAppDelegate() {
	return _delegate;
}

void VRApp::loadLuaFile(const std::string & fn) {
	_luaFn = fn;

	// Run the Lua file
	_luaScript.runFile(_luaFn);

}

void VRApp::initScene() {
	std::cout << "---------- VRApp::initScene() ---------------" << std::endl;
	// Initialize devices
	const std::string wand("VJWand");
	m_wand.init(wand);

	// Create the top level node of the tree
	m_rootNode = new osg::Group();
}

void VRApp::configSceneView(osgUtil::SceneView* newSceneViewer) {
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
	this->getScene()->addChild( lightSource0.get() );
}

void VRApp::preFrame() {
	/*
	if (m_logicManager.isEmpty()) {
		/// Activate frame logic in primary state machine
		assert(m_pStateMachine.valid());
		m_pStateMachine->activate();
	}
	*/
	vpr::Interval cur_time = m_wand->getTimeStamp();
	vpr::Interval diff_time(cur_time - m_lastPreFrameTime);
	if (m_lastPreFrameTime.getBaseVal() >= cur_time.getBaseVal()) {
		diff_time.secf(0.0f);
	}

	m_timeDelta = diff_time.secf();

	/// Pass this message on to all of our frame logic units.
	//m_logicManager.callOnAllFrameLogicUnits(&FrameLogicUnit::preFrame);

	m_lastPreFrameTime = cur_time;
}

void VRApp::latePreFrame() {

	/// Update the scene graph here!

	/// Pass this message on to all of our frame logic units.
	//m_logicManager.callOnAllFrameLogicUnits(&FrameLogicUnit::latePreFrame);

	// Finish updating the scene graph.
	vrj::OsgApp::latePreFrame();
}

void VRApp::intraFrame() {
	/// Pass this message on to all of our frame logic units.
	//m_logicManager.callOnAllFrameLogicUnits(&FrameLogicUnit::intraFrame);
}

void VRApp::postFrame() {
	/// Pass this message on to all of our frame logic units.
	//m_logicManager.callOnAllFrameLogicUnits(&FrameLogicUnit::postFrame);
}
