/**	@file	LuaOSGApp.h
	@brief Application object header

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
#ifndef INCLUDED_vrjugglua_LuaOSGApp_h
#define INCLUDED_vrjugglua_LuaOSGApp_h

/////////////
// Local includes
#include <vrjugglua/LuaScript.h>

/////////////
// Library/third-party includes

// LOVE includes
//#include <love/FrameStateMachine.h>
//#include <love/FrameLogicManager.h>

// OSG includes
#include <osg/Group>
#include <osgUtil/SceneView>

// VR Juggler includes
#include <vrj/vrjConfig.h>
#include <vrj/Draw/OSG/OsgApp.h>
#include <gadget/Type/PositionInterface.h>

// LuaBind includes
#include <luabind/object.hpp>

/////////////
// Standard includes
#include <string>
#include <vector>

class VRApp : public vrj::OsgApp {
	public:
	VRApp(vrj::Kernel* kern);

	virtual ~VRApp();

	void setAppDelegate(luabind::object const & delegate);
	luabind::object const & getAppDelegate();

	/** Static accessor for the app pointer.
		@returns the pointer to the singleton app object.
	*/
	static VRApp* getApp();

	/** Accessor for the scene root node
		@returns a pointer to the scene root group.
	*/
	virtual osg::Group* getScene();

	/** Interval between the most recent (current) preFrame and the previous.

		This is an accessor whose data member is updated in preFrame before
		calling any FrameLogicUnits.
		@returns a float representing the time interval.
	*/
	float getTimeDelta();

	void loadLuaFile(const std::string & fn);

	/** Execute any initialization needed before the API is started.

		This is called once before OSG is initialized.
	 */
	virtual void initScene();

	/** Set up the properties for our SceneView (lighting, etc)
	*/
	virtual void configSceneView(osgUtil::SceneView* newSceneViewer);

	/** This method is overridden to fix per-wall lighting issue in
		CAVEs.
	 */
	unsigned int getSceneViewDefaults();

	// ----- Drawing Loop Functions ------
	//
	//  The drawing loop will look similar to this:
	//
	//  while (drawing)
	//  {
	//        preFrame();
	//       <Application Data Syncronization>
	//        latePreFrame();
	//       draw();
	//        intraFrame();     // Drawing is happening while here
	//       sync();
	//        postFrame();      // Drawing is now done
	//
	//       UpdateTrackers();
	//  }
	//------------------------------------

	/** Called after getting input data but before syncing shared data.

		In this implementation, our timeDelta is updated then all of
		the frame logic units are called.

		This is the best time to call "setNextState()"

		@warning Any operation placed here increases total input latency!
	*/
	virtual void preFrame();

	/** Called after getting input data and syncing shared data.

		In this implementation, all of the frame logic units are called,
		then the parent class OsgApp's implementation is called.

		@warning Any operation placed here increases total input latency!
	*/
	virtual void latePreFrame();

	/** Called after drawing starts, during the draw process.

		In this implementation, all of the frame logic units are called.

		@warning Do not modify any information used in drawing here!
	*/
	virtual void intraFrame();

	/** Called once drawing has concluded, before getting the next inputs.

		In this implementation, all of the frame logic units are called.
		This means that the first thing that happens is that the primary
		state machine will advance to its next state (calling the go()
		method), then all other frame logic will take place.

		(All FrameStateMachines will also trigger their go() method here,
		mixed in with the rest of the Frame Logic Units in order of
		registration.)
	*/
	virtual void postFrame();


	gadget::PositionInterface m_wand; /**< the wand, used for timestamps */

	float m_timeDelta;

	protected:
	/**	Static app pointer - do not access directly	*/
	static VRApp* m_pApp;

	//FrameStateMachinePtr m_pStateMachine;

	private:
	osg::ref_ptr<osg::Group>           m_rootNode;

	luabind::object _delegate;

	/**
		Time of the start of the last preframe.

		Updated each preframe using the wand timestamp
	*/
	vpr::Interval m_lastPreFrameTime;

	std::string _luaFn;
	vrjLua::LuaScript _luaScript;
};

// -- inline implementations -- /
inline VRApp* VRApp::getApp() {
	return m_pApp;
}

inline osg::Group* VRApp::getScene() {
	return m_rootNode.get();
}

inline unsigned int VRApp::getSceneViewDefaults() {
	return osgUtil::SceneView::NO_SCENEVIEW_LIGHT;
}

inline float VRApp::getTimeDelta() {
	return m_timeDelta;
}

#endif // INCLUDED_vrjugglua_LuaOSGApp_h
