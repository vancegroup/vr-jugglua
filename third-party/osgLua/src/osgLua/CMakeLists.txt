# Embedded build for osgLua
# 2009-2010 Ryan Pavlik <rpavlik@iastate.edu>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

set(OSGLUA_SRCS
	BindValue.cpp
	BindValue.h
	Callback.cpp
	lua_functions.cpp
	osgLua.cpp
	Script.cpp
	Type.cpp
	Value.cpp
	Value_rawget.inl
	Value_metamethods.cpp
	Value_metamethods.h)

set(OSGLUA_API
	lua_functions.h
	osgLua.h
	Type.h)

set(OSGLUA_SIMPLE_API
	../../include/osgLua/Callback
	../../include/osgLua/LuaInclude
	../../include/osgLua/Script
	../../include/osgLua/Value)

source_group(API FILES ${OSGLUA_SIMPLE_API})


if(BUILD_SHARED_OSGLUA)
	add_library(osgLua SHARED ${OSGLUA_SRCS} ${OSGLUA_API} ${OSGLUA_SIMPLE_API})
	target_link_libraries(osgLua lua ${OPENSCENEGRAPH_LIBRARIES} lua)
endif()

if(BUILD_LUA_AS_CPP)
	add_definitions(-DBUILD_LUA_AS_CPP)
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
	"Third-Party: OsgLua Static Library")

if(INSTALL_CXX_SDK)
	install(TARGETS osgLua_static
		EXPORT vrjlua-sdk
		RUNTIME DESTINATION ${BIN_DIR} COMPONENT runtime
		LIBRARY DESTINATION ${LIB_DIR} COMPONENT runtime
		ARCHIVE DESTINATION ${ARCH_DIR} COMPONENT sdk
		PUBLIC_HEADER DESTINATION ${INCLUDE_DIR}/osgLua COMPONENT sdk)
endif()
