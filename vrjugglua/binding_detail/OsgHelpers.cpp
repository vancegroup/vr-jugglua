/**	@file	binding_detail/OsgHelpers.cpp
	@brief	implementation of manual functions and bindings for OSG

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
#include "OsgHelpers.h"

#include "../LuaIncludeFull.h"

// Library/third-party includes
#include <osg/ComputeBoundsVisitor>

#include <osgLua/Value>

#include <osgLua/IntrospectionValue>
#include <osgLua/Introspection_variant_cast>

// Standard includes
#include <iostream>


namespace vrjLua {



	int computeBoundingBox(lua_State *L) {
		int n = lua_gettop(L);
		if (n != 1) {
			lua_pushstring(L, "Must pass only a single argument: a node");
			lua_error(L);
		}

		// grab the param
		osgLua::Value* v = osgLua::Value::get(L, n);
		if (!v) {
			lua_pushstring(L, "Could not grab osgLua value off of the stack!");
			lua_error(L);
		}

		osg::Node * node;

		node = osgIntrospection::variant_cast<osg::Node*>(v->get());

		if (!node) {
			lua_pushstring(L, "Could not convert argument 1 to an osg::Node* !");
			lua_error(L);
		}

#ifdef BUILD_VERBOSE
		std::cout << "OK, we successfully did a variant_cast" << std::endl;
#endif

		osg::ComputeBoundsVisitor * visitor = new osg::ComputeBoundsVisitor();
		node->accept(*visitor);

		osg::BoundingBox & bbox = visitor->getBoundingBox();

		lua_newtable(L);

		// push key, push value, set table

		lua_pushstring(L, "xMin");
		lua_pushnumber(L, bbox.xMin());
		lua_settable(L, -3);

		lua_pushstring(L, "xMax");
		lua_pushnumber(L, bbox.xMax());
		lua_settable(L, -3);

		lua_pushstring(L, "yMin");
		lua_pushnumber(L, bbox.yMin());
		lua_settable(L, -3);

		lua_pushstring(L, "yMax");
		lua_pushnumber(L, bbox.yMax());
		lua_settable(L, -3);

		lua_pushstring(L, "zMin");
		lua_pushnumber(L, bbox.zMin());
		lua_settable(L, -3);

		lua_pushstring(L, "zMax");
		lua_pushnumber(L, bbox.zMax());
		lua_settable(L, -3);

		lua_pushstring(L, "radius");
		lua_pushnumber(L, bbox.radius());
		lua_settable(L, -3);

		return 1;

	}

	void manuallyBindOsgHelpers(LuaStatePtr state) {
		lua_register(state.get(), "getBoundingBox", &computeBoundingBox);
	}

}// end of vrjLua namespace
