# Embedded build for osgLua
# 2009-2010 Ryan Pavlik <rpavlik@iastate.edu>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

set(OSGLUA_SRCS
	Callback.cpp
	lua_functions.cpp
	osgLua.cpp
	Script.cpp
	Type.cpp
	Value.cpp)

set(OSGLUA_API
	lua_functions.h
	osgLua.h
	Type.h
	Value.h)

set(OSGLUA_SIMPLE_API
	../../include/osgLua/Callback
	../../include/osgLua/Script)

source_group(API FILES ${OSGLUA_SIMPLE_API})


if(BUILD_SHARED_OSGLUA)
	add_library(osgLua SHARED ${OSGLUA_SRCS} ${OSGLUA_API} ${OSGLUA_SIMPLE_API})
	target_link_libraries(osgLua lua ${OPENSCENEGRAPH_LIBRARIES} lua)
endif()

# TODO: this might make it impossible to build an osgLua dll on windows
add_definitions(-DOSGLUA_LIBRARY_STATIC)
add_library(osgLua_static ${OSGLUA_SRCS} ${OSGLUA_API} ${OSGLUA_SIMPLE_API})
set_target_properties(osgLua_static PROPERTIES PUBLIC_HEADER "${OSGLUA_API};${OSGLUA_SIMPLE_API}")
target_link_libraries(osgLua_static lua ${OPENSCENEGRAPH_LIBRARIES} lua)

set_property(TARGET
	osgLua_static
	PROPERTY
	PROJECT_LABEL
	"OsgLua Static Library")

if(INSTALL_CXX_SDK)
	install(TARGETS osgLua_static
		EXPORT vrjlua-sdk
		ARCHIVE DESTINATION ${LIB_DIR}
		PUBLIC_HEADER DESTINATION ${INCLUDE_DIR}/osgLua)
endif()