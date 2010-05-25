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


inline osg::Group* LuaOsgApp::getScene() {
	return _root.get();
}

inline void LuaOsgApp::init() {
	_root = new osg::Group();
	vrj::OsgApp::init();
}

inline void LuaOsgApp::initScene() { ; }

inline unsigned int LuaOsgApp::getSceneViewDefaults() {
	return osgUtil::SceneView::NO_SCENEVIEW_LIGHT;
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

void bindOsgAppToLua(LuaStatePtr state);


#undef BASECLASS
#undef STATIC_METHOD
}// end of vrjLua namespace


#endif // INCLUDED_vrjugglua_BindPositionInterfaceToLua_h
