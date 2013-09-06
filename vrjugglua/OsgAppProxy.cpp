/**	@file
	@brief Application proxy object implementation

	@date
	2009-2011

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

//          Copyright Iowa State University 2009-2011.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


// Internal Includes
#include "OsgAppProxy.h"

#include "osgLuaBind.h"
#include "VRJLuaOutput.h"
#include "LuaScript.h"

// Library/third-party includes
#include <luabind/class.hpp>

#include <osg/PositionAttitudeTransform>
#include <osg/Group>
#include <osgUtil/SceneView>

#include <vrj/Kernel/Kernel.h>          // for Kernel
#include <vrj/Kernel/User.h>            // for User

// Standard includes
#include <cstdlib>

namespace vrjLua {

	static const double FEET_PER_METER = 3.2808399;
/// Initialize static member variable
	OsgAppProxy* OsgAppProxy::_pApp = NULL;

	void OsgAppProxy::bindToLua(lua_State * L) {
		// Bind this class
		if (L) {
#ifdef VERBOSE
			VRJLUA_MSG_START(dbgVRJLUA_PROXY, MSG_STATUS)
			        << "Registering vrj.OsgAppProxy with Lua"
			        << VRJLUA_MSG_END(dbgVRJLUA_PROXY, MSG_STATUS);
#endif
			using namespace luabind;
			module(L, "vrjApp") [
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

	OsgAppProxy::OsgAppProxy() :
		vrj::OsgApp(vrj::Kernel::instance()),
		_delegationSuccessFlag(true),
		_timeDelta(-1) {
		/// update static pointer to app object
		_pApp = this;
	}

	OsgAppProxy::OsgAppProxy(vrj::Kernel* kern/*, int & argc, char** argv*/) :
		vrj::OsgApp(kern),
		_delegationSuccessFlag(true),
		_timeDelta(-1) {
		/// update static pointer to app object
		_pApp = this;

	}

	OsgAppProxy::~OsgAppProxy() {
#ifdef VERBOSE
		std::cout << "In destructor " << __FUNCTION__ << std::endl;
#endif
		_forwardCallToDelegate("exit");

		// Delete the delegate object so that it doesn't call a destroyed state
		// to clean itself up later
		_delegate = luabind::object();
	}

	void OsgAppProxy::setActiveApplication() {
		vrj::Kernel::instance()->setApplication(this);
	}

	bool OsgAppProxy::setAppDelegate(luabind::object const & delegate) {
		/// @todo test here to see if the passed delegate is suitable

		if (luabind::type(delegate) == LUA_TTABLE) {
#ifdef VERBOSE
			VRJLUA_MSG_START(dbgVRJLUA_PROXY, MSG_STATUS)
			        << "App delegate set."
			        << VRJLUA_MSG_END(dbgVRJLUA_PROXY, MSG_STATUS);
#endif
			_delegate = delegate;

			// New delegate gets a fresh attempt
			_delegationSuccessFlag = true;
			return true;
		} else {
			VRJLUA_MSG_START(dbgVRJLUA_PROXY, MSG_ERROR)
			        << "Lua app tried to set an invalid app delegate: " << delegate
			        << VRJLUA_MSG_END(dbgVRJLUA_PROXY, MSG_ERROR);
			return false;
		}
	}

	luabind::object const & OsgAppProxy::getAppDelegate() {
		return _delegate;
	}

	void OsgAppProxy::initScene() {
#ifdef VERBOSE
		VRJLUA_MSG_START(dbgVRJLUA_PROXY, MSG_STATUS)
		        << "------- OsgAppProxy::initScene() -------"
		        << VRJLUA_MSG_END(dbgVRJLUA_PROXY, MSG_STATUS);
#endif

		// Exit now if there's no delegate set
		if (!_delegate) {
			VRJLUA_MSG_START(dbgVRJLUA_PROXY, MSG_WARNING)
			        << "No delegate has been set yet - exiting to avoid busy-waiting forever."
			        << VRJLUA_MSG_END(dbgVRJLUA_PROXY, MSG_WARNING);
			//vrj::Kernel::instance()->stop();
			std::exit(1);
		}

		// Create the top level node of the tree, transforming
		// so that user apps use meters.
		osg::ref_ptr<osg::PositionAttitudeTransform> xform = new osg::PositionAttitudeTransform();
		xform->setScale(osg::Vec3d(FEET_PER_METER, FEET_PER_METER, FEET_PER_METER));
		_rootNode = xform.get();

		VRJLUA_MSG_START(dbgVRJLUA_PROXY, MSG_STATUS)
		        << "Number of children before forwarding call to delegate: " << _rootNode->getNumChildren()
		        << VRJLUA_MSG_END(dbgVRJLUA_PROXY, MSG_STATUS);

		_forwardCallToDelegate("initScene", true); // required call - exceptions propagate

		VRJLUA_MSG_START(dbgVRJLUA_PROXY, MSG_STATUS)
		        << "Number of children after forwarding call: " << _rootNode->getNumChildren()
		        << VRJLUA_MSG_END(dbgVRJLUA_PROXY, MSG_STATUS);
	}

	void OsgAppProxy::configSceneView(osgUtil::SceneView* newSceneViewer) {
		vrj::OsgApp::configSceneView(newSceneViewer);
		/*
		osg::ref_ptr<osg::Light> light = newSceneViewer->getLight();
		if (light) {
			light->setAmbient(osg::Vec4(0.0f,0.0f,0.0f,1.0f));
			light->setDiffuse(osg::Vec4(0.5f,0.5f,0.5f,1.0f));
			light->setSpecular(osg::Vec4(0.5f,0.5f,0.5f,1.0f));
		}
		*/

		// setup the ambient light the way I want it
		/*
		osg::LightModel* lightmodel = new osg::LightModel;
		lightmodel->setAmbientIntensity(osg::Vec4(0.5f,0.5f,0.5f,0.5f));

		newSceneViewer->getGlobalStateSet()->setAttributeAndModes(lightmodel, osg::StateAttribute::ON);
		*/

	}

	void OsgAppProxy::preFrame() {
		if (!_delegationSuccessFlag) {
			// We made it through a whole frame without a successful call
			// to the delegate - bail out.
			VRJLUA_MSG_START(dbgVRJLUA_PROXY, MSG_ERROR)
			        << "A full frame loop completed without any successful calls to the delegate."
			        << VRJLUA_MSG_END(dbgVRJLUA_PROXY, MSG_ERROR);
			VRJLUA_MSG_START(dbgVRJLUA_PROXY, MSG_ERROR)
			        << "Assuming application failure, bailing out."
			        << VRJLUA_MSG_END(dbgVRJLUA_PROXY, MSG_ERROR);
			vrj::Kernel::instance()->stop();
			std::exit(1);
		}

		_delegationSuccessFlag = false;
		vpr::Interval cur_time = _lastPreFrameTime;
		if (vrj::Kernel::instance()->getUsers().size() > 0) {
			cur_time = vrj::Kernel::instance()->getUsers()[0]->getHeadUpdateTime();
		}
		vpr::Interval diff_time(cur_time - _lastPreFrameTime);
		if (_lastPreFrameTime.getBaseVal() >= cur_time.getBaseVal()) {
			diff_time.secf(0.0f);
		}

		_timeDelta = diff_time.secd();

		_forwardCallToDelegate("preFrame");

		_lastPreFrameTime = cur_time;
	}

	void OsgAppProxy::latePreFrame() {
		_forwardCallToDelegate("latePreFrame");

		// Finish updating the scene graph.
		vrj::OsgApp::latePreFrame();
	}

	void OsgAppProxy::intraFrame() {
		_forwardCallToDelegate("intraFrame");
	}

	void OsgAppProxy::postFrame() {
		_forwardCallToDelegate("postFrame");
	}

	unsigned int OsgAppProxy::getSceneViewDefaults() {
		return (osgUtil::SceneView::SKY_LIGHT | osgUtil::SceneView::COMPILE_GLOBJECTS_AT_INIT);
		//return osgUtil::SceneView::NO_SCENEVIEW_LIGHT;
	}

	osg::Group* OsgAppProxy::getScene() {
		return _rootNode.get();
	}

	double OsgAppProxy::getTimeDelta() {
		return _timeDelta;
	}

	bool OsgAppProxy::_forwardCallToDelegate(const char * call, bool required) {
		if (_delegate && luabind::type(_delegate[call]) == LUA_TFUNCTION) {
			try {
				_delegate[call](_delegate);

				// We had at least one success this frame
				_delegationSuccessFlag = true;
				return true;
			} catch (luabind::error & e) {
				std::stringstream s;
				s << "Calling '" << call << "' in the app delegate failed - check your lua code!" << std::endl;
				luabind::object o(luabind::from_stack(e.state(), -1));
				s << "Top of the Lua stack (error message) is: '" << o << "'";
				LuaScript::doPrint(s.str());
				if (required) {
					std::exit(1);
					//throw;
					vrj::Kernel::instance()->stop();
				}
			}
			return false;
		}
#ifdef VERY_VERBOSE
		VRJLUA_MSG_START(dbgVRJLUA_PROXY, MSG_DRIVEL)
		        << "Delegate not valid or no '" << call << "' element defined: " << _delegate
		        << VRJLUA_MSG_END(dbgVRJLUA_PROXY, MSG_DRIVEL);
#endif
		return false;
	}

} // end of vrjLua namespace
