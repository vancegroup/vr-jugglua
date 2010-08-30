/**	@file	binding_detail/BindOsgToLua.cpp
	@brief	implementation of binding using osgLua

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
#include "BindOsgToLua.h"

#include "../LuaIncludeFull.h"

// Library/third-party includes
#include <osgLua/osgLua.h>
#include <osgLua/Value.h>

#include <luabind/luabind.hpp>

#include <osg/ComputeBoundsVisitor>

#include <osgIntrospection/Value>
#include <osgIntrospection/variant_cast>

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

	osgIntrospection::Type const& t = v->get().getInstanceType();
	osg::Node * node;
/*
	if (t.isSubclassOf(osgIntrospection::Reflection::getType("osg::Node"))) {
		if (v->getType().isPointer()) {
		*/
			node = osgIntrospection::variant_cast<osg::Node*>(v->get());
			/*
		} else if (v->getType().isReference()) {
			osg::Node& n = osgIntrospection::variant_cast<osg::Node&>(v->get());
			node = &n;
		}
	}*/
	if (!node) {
		//throw new std::runtime_error("Could not convert argument 1 to an osg::Node* !");
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

static const struct luaL_reg osgHelpers [] = {
      {"computeBoundingBox", &computeBoundingBox},
      {NULL, NULL}  /* sentinel */
    };


void bindOsgToLua(LuaStatePtr state) {
#ifdef VERBOSE
	std::cerr << "Started binding OSG to Lua..." << std::flush << std::endl;
#endif
	luaopen_osgLua(state.get());
#ifdef VERBOSE
	luaL_dostring(state.get(), "print('osgLua: ' .. tostring(osgLua.getTypes()))");
#endif
	bool ret;

#ifdef AUTOLOAD_WRAPPER_OSG
	ret = osgLua::loadWrapper(state.get(), "osg");
	if (!ret) {
		std::cerr << "Could not load Lua wrapper for osg!" << std::endl;
		return;
	}
#endif

#ifdef AUTOLOAD_WRAPPER_OSGDB
	ret = osgLua::loadWrapper(state.get(), "osgDB");
	if (!ret) {
		std::cerr << "Could not load Lua wrapper for osgDB!" << std::endl;
		return;
	}
#endif

#ifdef AUTOLOAD_WRAPPER_OSGUTIL
	ret = osgLua::loadWrapper(state.get(), "osgUtil");
	if (!ret) {
		std::cerr << "Could not load Lua wrapper for osgUtil!" << std::endl;
		return;
	}
#endif
/*
	luabind::module(state.get(), "osgHelpers") [
	    luabind::class_<BoundingBoxFinder>("BoundingBoxFinder")
	    .def(luabind::constructor<NodePtr>())
	    .def("run", &BoundingBoxFinder::run)
	    .def_readwrite("minX", &BoundingBoxFinder::minX)
	    .def_readwrite("maxX", &BoundingBoxFinder::maxX)
	    .def_readwrite("minY", &BoundingBoxFinder::minY)
	    .def_readwrite("maxY", &BoundingBoxFinder::maxY)
	    .def_readwrite("minZ", &BoundingBoxFinder::minZ)
	    .def_readwrite("maxZ", &BoundingBoxFinder::maxZ)


	    //def("decodeBoundingBox", & decodeBoundingBox)
	];
	*/

	// Bind the helper function
	//luaL_openlib(state.get(), "osgHelpers", osgHelpers, 0);
	lua_register(state.get(), "getBoundingBox", &computeBoundingBox);
}

}// end of vrjLua namespace
