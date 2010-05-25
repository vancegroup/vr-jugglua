# Embedded build for osgLua
# 2009-2010 Ryan Pavlik <rpavlik@iastate.edu>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

set(OSGLUA_SRCS
	Callback.cpp
	lua_functions.cpp
	lua_functions.h
	osgLua.cpp
	osgLua.h
	Script.cpp
	Type.cpp
	Type.h
	Value.cpp
	Value.h
	../../include/osgLua/Script)

#add_library(osgLua SHARED ${OSGLUA_SRCS})
add_library(osgLua ${OSGLUA_SRCS})
target_link_libraries(osgLua ${OPENSCENEGRAPH_LIBRARIES} ${LUA_LIBRARIES})
