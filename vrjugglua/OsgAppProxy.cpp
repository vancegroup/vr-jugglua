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

#include "ConvertOsgPtrToOsgLuaPtr.h"

#include "VRJLuaOutput.h"
#include <util/osg/gmtlToOsgMatrix.h>

// Library/third-party includes
#include <luabind/class.hpp>

#include <osg/LightModel>

#include <osgDB/Registry>
#include <osg/MatrixTransform>

// Standard includes
#include <cstring>
#include <cstdlib>

namespace vrjLua {
/// Initialize static member variable
OsgAppProxy* OsgAppProxy::_pApp = NULL;

void OsgAppProxy::bindToLua(LuaStatePtr & state) {
	// Bind this class
	if (state) {
#ifdef VERBOSE
		VRJLUA_MSG_START(dbgVRJLUA_PROXY, MSG_STATUS)
			<< "Registering vrj.OsgAppProxy with Lua"
			<< VRJLUA_MSG_END(dbgVRJLUA_PROXY, MSG_STATUS);
#endif
		using namespace luabind;
		module(state.get(), "vrjApp") [
			class_<OsgAppProxy>("OsgAppProxy")
				.def(constructor<>())
				.def("setAppDelegate", & OsgAppProxy::setAppDelegate)
				.def("getAppDelegate", & OsgAppProxy::getAppDelegate)
				.def("setActiveApplication", & OsgAppProxy::setActiveApplication)
				.def("getScene", & OsgAppProxy::getScene)
				.def("getNodeTrackingPositionOnly", &OsgAppProxy::getNodeTrackingPositionOnly)
				.def("getNodeTrackingPose", &OsgAppProxy::getNodeTrackingPose)
				.def("getTimeDelta", & OsgAppProxy::getTimeDelta)
				.def("addModelSearchPath", &OsgAppProxy::addModelSearchPath)
	   ];
	}
}

OsgAppProxy::OsgAppProxy() :
		vrj::OsgApp(vrj::Kernel::instance()),
		_timeDelta(-1),
		_delegationSuccessFlag(true) {
	/// update static pointer to app object
	_pApp = this;
}

OsgAppProxy::OsgAppProxy(vrj::Kernel* kern/*, int & argc, char** argv*/) :
		vrj::OsgApp(kern),
		_timeDelta(-1),
		_delegationSuccessFlag(true) {
	/// update static pointer to app object
	_pApp = this;

}

OsgAppProxy::~OsgAppProxy() {
#ifdef VERBOSE
	std::cout << "In destructor " << __FUNCTION__ << std::endl;
#endif
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

	_lastPreFrameTime.setNow();

	// Exit now if there's no delegate set
	if (!_delegate) {
		VRJLUA_MSG_START(dbgVRJLUA_PROXY, MSG_WARNING)
			<< "No delegate has been set yet - exiting to avoid busy-waiting forever."
			<< VRJLUA_MSG_END(dbgVRJLUA_PROXY, MSG_WARNING);
		//vrj::Kernel::instance()->stop();
		std::exit(1);
	}

	// Create the top level node of the tree
	_rootNode = new osg::Group();
	VRJLUA_MSG_START(dbgVRJLUA_PROXY, MSG_STATUS)
		<< "Number of children before forwarding call to delegate: " << _rootNode->getNumChildren()
		<< VRJLUA_MSG_END(dbgVRJLUA_PROXY, MSG_STATUS);

	_forwardCallToDelegate("initScene", true); // required call - exceptions propagate

	VRJLUA_MSG_START(dbgVRJLUA_PROXY, MSG_STATUS)
			<< "Number of children after forwarding call: " << _rootNode->getNumChildren()
			<< VRJLUA_MSG_END(dbgVRJLUA_PROXY, MSG_STATUS);
}

void OsgAppProxy::addModelSearchPath(std::string const& path) {
	osgDB::Registry::instance()->getDataFilePathList().push_back(path);
}

void OsgAppProxy::configSceneView(osgUtil::SceneView* newSceneViewer) {
	vrj::OsgApp::configSceneView(newSceneViewer);
	newSceneViewer->getLight()->setAmbient(osg::Vec4(0.5f,0.5f,0.5f,0.5f));
	newSceneViewer->getLight()->setDiffuse(osg::Vec4(0.5f,0.5f,0.5f,0.5f));
	newSceneViewer->getLight()->setSpecular(osg::Vec4(0.5f,0.5f,0.5f,0.5f));

	// setup the ambient light the way I want it
	osg::LightModel* lightmodel = new osg::LightModel;
	lightmodel->setAmbientIntensity(osg::Vec4(0.5f,0.5f,0.5f,0.5f));

	newSceneViewer->getGlobalStateSet()->setAttributeAndModes(lightmodel, osg::StateAttribute::ON);
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
	vpr::Interval cur_time = vrj::Kernel::instance()->getUsers()[0]->getHeadUpdateTime();
	vpr::Interval diff_time(cur_time - _lastPreFrameTime);
	if (_lastPreFrameTime.getBaseVal() >= cur_time.getBaseVal()) {
		diff_time.secf(0.0f);
	}

	_timeDelta = diff_time.secd();

	const unsigned int n = _trackNodes.size();
	for (unsigned int i = 0; i < n; ++i) {
		_trackNodes[i]->update();
	}

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

osg::Group* OsgAppProxy::getScene() {
	return _rootNode.get();
}

OsgAppProxy::PositionTrackingNode::PositionTrackingNode(std::string const& devName, bool posOnly) :
		deviceName(devName),
		positionOnly(posOnly) {
	device.init(deviceName);
	node = new osg::MatrixTransform;
}

void OsgAppProxy::PositionTrackingNode::update() {
	osg::Matrix mat(util::osg::toOsgMatrix(device->getData(gadget::PositionUnitConversion::ConvertToMeters)));
	if (positionOnly) {
		osg::Matrix transMat;
		transMat.setTrans(mat.getTrans());
		dynamic_cast<osg::MatrixTransform*>(node.get())->setMatrix(transMat);
	} else {
		dynamic_cast<osg::MatrixTransform*>(node.get())->setMatrix(mat);
	}
}

osg::Group* OsgAppProxy::getNodeTrackingPositionOnly(std::string const& device) {
	for (unsigned int i = 0; i < _trackNodes.size(); ++i) {
		if (_trackNodes[i]->is(device, true)) {
			std::cout << "Found existing node tracking the position of " << device << " with node="<<_trackNodes[i]->node.get()<<std::endl;
			return _trackNodes[i]->node.get();
		}
	}
	boost::shared_ptr<PositionTrackingNode> temp(new PositionTrackingNode(device, true));
	std::cout << "Creating node tracking the position of " << device << " with node="<<temp->node.get()<<std::endl;
	_trackNodes.push_back(temp);
	return temp->node.get();
}

osg::Group* OsgAppProxy::getNodeTrackingPose(std::string const& device) {
	for (unsigned int i = 0; i < _trackNodes.size(); ++i) {
		if (_trackNodes[i]->is(device, false)) {
			std::cout << "Found existing node tracking the full pose of " << device << " with node="<<_trackNodes[i]->node.get()<<std::endl;
			return _trackNodes[i]->node.get();
		}
	}
	boost::shared_ptr<PositionTrackingNode> temp(new PositionTrackingNode(device, false));
	std::cout << "Creating node tracking the full pose of " << device << " with node="<<temp->node.get()<<std::endl;
	_trackNodes.push_back(temp);
	return temp->node.get();
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
			VRJLUA_MSG_START(dbgVRJLUA_PROXY, MSG_ERROR)
				<< "Calling '" << call << "' in the delegate failed - check your lua code!"
				<< VRJLUA_MSG_END(dbgVRJLUA_PROXY, MSG_ERROR);
			luabind::object o(luabind::from_stack(e.state(), -1));
			VRJLUA_MSG_START(dbgVRJLUA_PROXY, MSG_ERROR)
				<< "Top of the Lua stack (error message) is: '" << o << "'"
				<< VRJLUA_MSG_END(dbgVRJLUA_PROXY, MSG_ERROR);
			if (required) {
				throw;
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
