/*
	osgLua: use Lua to access dynamically to osg using osgIntrospection
	Copyright(C) 2006 Jose L. Hidalgo Valiño (PpluX) (pplux at pplux.com)
	Copyright(C) 2010-2011 Iowa State University (Author: Ryan Pavlik <rpavlik@acm.org> )

    This library is open source and may be redistributed and/or modified under
    the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or
    (at your option) any later version.  The full license is in LICENSE file
    included with this distribution, and on the openscenegraph.org website.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    OpenSceneGraph Public License for more details.
*/

#include "osgLua.h"
#include "LuaIncludeFull.h"
#include <osgLua/Value>

#include <osgDB/WriteFile>
#include <osgIntrospection/variant_cast>
#include <osg/Object>

int osgLua::lua_saveObjectFile(lua_State *L) {
	osgLua::Value * v = osgLua::Value::getRequired(L, 1);

	const char *name = lua_tostring(L,2);
	luaL_argcheck(L, name != 0, 2, "need a string");
	bool success = osgDB::writeObjectFile( *osgIntrospection::variant_cast<osg::Object const*>(v->get()), name);
	if (!success) {
		luaL_error(L, "Could not write osgLua object to file %s", name);
	}

	return 0;
}

