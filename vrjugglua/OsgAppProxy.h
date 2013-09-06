/**	@file
	@brief Application proxy object header

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

#pragma once
#ifndef INCLUDED_vrjugglua_OsgAppProxy_h
#define INCLUDED_vrjugglua_OsgAppProxy_h


// Local includes
#include <vrjugglua/LuaScript.h>

// OSG includes
#include <osg/ref_ptr>

// VR Juggler includes
#include <vrj/vrjParam.h>
#include <vpr/Util/Interval.h>

#if __VJ_version < 2003011
#	include <vrj/Draw/OSG/OsgApp.h>
typedef vrj::OsgApp OsgApp;
#else
#	include <vrj/Draw/OSG/App.h>
typedef vrj::osg::App OsgApp;
#endif


// LuaBind includes
#include <luabind/object.hpp>

// Standard includes
// - none

namespace osg {
	class Group;
}
namespace osgUtil {
	class SceneView;
}
namespace vrj {
	class Kernel;
}

namespace vrjLua {

	class OsgAppProxy : public OsgApp {
		public:
			/// @brief Lua binding call
			static void bindToLua(lua_State * L);

			/// @brief Standard constructor
			OsgAppProxy(vrj::Kernel* kern);

			/// @brief simple destructor
			virtual ~OsgAppProxy();

			/// @name Lua-accessible methods
			/// @{

			/// @brief Default constructor, called from Lua
			OsgAppProxy();

			/// @brief Set this application as the current one in the kernel
			void setActiveApplication();

			/// @brief Set the Lua table serving as the app delegate
			bool setAppDelegate(luabind::object const & delegate);

			/// @brief Retrieve the Lua table serving as the app delegate
			luabind::object const & getAppDelegate();

			/** Accessor for the scene root node
				@returns a pointer to the scene root group.
			*/
			virtual osg::Group* getScene();

			/** Interval between the most recent (current) preFrame and the previous.

				This is an accessor whose data member is updated in preFrame before
				calling the delegate.
				@returns a float representing the time interval.
			*/
			double getTimeDelta();
			/// @}

			/// @name Methods for C++ access only
			/// @{
			/** Static accessor for the app pointer.
				@returns the pointer to the singleton app object.
			*/
			static OsgAppProxy* getApp();

			/** Set up the properties for our SceneView (lighting, etc)
			*/
			virtual void configSceneView(osgUtil::SceneView* newSceneViewer);

			/** This method is overridden to fix per-wall lighting issue in
				CAVEs.
			 */
			unsigned int getSceneViewDefaults();
			/// @}

			/// @name Methods proxied to the Lua delegate
			/// @{
			/** Execute any initialization needed before the API is started.

				This is called once before OSG is initialized.
			 */
			virtual void initScene();


			// ----- Drawing Loop Functions ------
			//
			//  The drawing loop will look similar to this:
			//
			//  while (drawing)
			//  {
			//        preFrame();
			//       <Application Data Synchronization>
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
			/// @}

		protected:
			/**	Static app pointer - do not access directly	*/
			static OsgAppProxy* _pApp;

		private:
			osg::ref_ptr<osg::Group>           _rootNode;

			/// @name Proxy/Delegation internals
			/// @{
			bool _forwardCallToDelegate(const char * call, bool required = false);
			luabind::object _delegate;
			bool _delegationSuccessFlag;
			/// @}

			/**	@brief Time of the start of the last preframe.

				Updated each preframe using the user's head timestamp
			*/
			vpr::Interval _lastPreFrameTime;
			double _timeDelta;
	};

// -- inline implementations -- //
// -- only safe for functions not callable from lua -- /
	inline OsgAppProxy* OsgAppProxy::getApp() {
		return _pApp;
	}


} // end of vrjLua namespace
#endif // INCLUDED_vrjugglua_LuaOSGApp_h
