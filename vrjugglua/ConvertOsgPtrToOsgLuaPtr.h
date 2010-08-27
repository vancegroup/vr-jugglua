/**	@file	ConvertOsgPtrToOsgLuaPtr.h
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
#ifndef INCLUDED_vrjugglua_ConvertOsgPtrToOsgLuaPtr_h
#define INCLUDED_vrjugglua_ConvertOsgPtrToOsgLuaPtr_h

// Local includes
// - none

// Library/third-party includes
#include <luabind/luabind.hpp>

#include <osgLua/Value.h>

#include <osg/Group>


// Standard includes
// - none

namespace luabind
{
    template <>
    struct default_converter<osg::Group*>
      : native_converter_base<osg::Group*>
    {
        static int compute_score(lua_State* L, int index)
        {
			/// @todo actually figure out what's good and what's not
        	return -1;
            return lua_type(L, index) == LUA_TUSERDATA ? 0 : -1;
        }

        osg::Group* from(lua_State* L, int index)
        {
        	/// @todo implement
            return NULL;
        }

        void to(lua_State* L, osg::Group* const& x)
        {
        	osgLua::Value::push(L, x);
        }
    };

    template <>
    struct default_converter<osg::Group* const&>
      : default_converter<osg::Group*>
    {};


    template <>
    struct default_converter<osg::Matrixd>
      : native_converter_base<osg::Matrixd>
    {
        static int compute_score(lua_State* L, int index)
        {
			/// @todo actually figure out what's good and what's not
        	return -1;
            return lua_type(L, index) == LUA_TUSERDATA ? 0 : -1;
        }

        osg::Group* from(lua_State* L, int index)
        {
        	/// @todo implement
            return NULL;
        }

        void to(lua_State* L, osg::Matrixd const& x)
        {
        	osgLua::Value::push(L, x);
        }
    };

    template <>
    struct default_converter<osg::Matrixd const&>
      : default_converter<osg::Matrixd>
    {};
}


#endif // INCLUDED_vrjugglua_LuaScript_h
