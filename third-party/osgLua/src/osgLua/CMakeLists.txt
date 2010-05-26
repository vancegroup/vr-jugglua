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
	Value.h)

set(OSGLUA_SIMPLE_API
	../../include/osgLua/Callback
	../../include/osgLua/Script)

source_group(API FILES ${OSGLUA_SIMPLE_API})


if(BUILD_SHARED_OSGLUA)
	add_library(osgLua SHARED ${OSGLUA_SRCS} ${OSGLUA_SIMPLE_API})
	target_link_libraries(osgLua lua ${OPENSCENEGRAPH_LIBRARIES} lua)
endif()

# TODO: this might make it impossible to build an osgLua dll on windows
add_definitions(-DOSGLUA_LIBRARY_STATIC)
add_library(osgLua_static ${OSGLUA_SRCS} ${OSGLUA_SIMPLE_API})
target_link_libraries(osgLua_static lua ${OPENSCENEGRAPH_LIBRARIES} lua)
