/**	@file	Lua_PositionInterface.h
	@brief	PositionInterface wrapper class for LuaBind

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
#include "LuaScript.h"

// Library/third-party includes
// - none

// Standard includes
// - none

namespace vrjLua {
	class FrameLogicUnit {
		public:
			PositionInterface();

			void init(const std::string & device);

			osg::Matrixf getMatrix();
			osg::Vec3f getPosition();
			osg::Vec3f getForwardVector();

		protected:
			gmtl::Matrix44f _getData(const float scale = gadget::PositionUnitConversion::ConvertToFeet);

			bool _ready;
			gadget::PositionInterface _iface;
	};

#ifndef LUABIND_COMBINED_COMPILE
	void bindPositionInterfaceToLua(LuaStatePtr state);
#endif
} // end of vrjLua namespace
