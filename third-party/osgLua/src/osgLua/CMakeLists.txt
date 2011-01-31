# Embedded build for osgLua
# 2009-2010 Ryan Pavlik <rpavlik@iastate.edu>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

set(OSGLUA_SRCS
	BindValue.cpp
	Callback.cpp
	lua_functions.cpp
	MathValueTags.cpp
	MatrixTemplates.h
	osgLua.cpp
	Script.cpp
	Type.cpp
	Value.cpp
	Value_rawget.inl
	Value_metamethod_members.cpp
	Value_metamethods.cpp
	Value_metamethods.h
	VectorTemplates.h)

set(OSGLUA_PRIVATE_API
	lua_functions.h
	LuaIncludeFull.h
	MathValueTags.h
	osgLua.h
	Type.h)

set(OSGLUA_PUBLIC_API
	../../include/osgLua/Callback
	../../include/osgLua/LoadWrapper
	../../include/osgLua/LuaInclude
	../../include/osgLua/Script
	../../include/osgLua/Value)

source_group(API FILES ${OSGLUA_PUBLIC_API})


if(BUILD_LUA_AS_CPP)
	add_definitions(-DOSGLUA_LUA_BUILT_AS_CPP)
endif()

if(BUILD_SHARED_OSGLUA)
	add_library(osgLua SHARED ${OSGLUA_SRCS} ${OSGLUA_PRIVATE_API} ${OSGLUA_PUBLIC_API})
	target_link_libraries(osgLua lua ${OPENSCENEGRAPH_LIBRARIES} lua)
endif()

# TODO: this might make it impossible to build an osgLua dll on windows
add_definitions(-DOSGLUA_LIBRARY_STATIC)
add_library(osgLua_static ${OSGLUA_SRCS} ${OSGLUA_PRIVATE_API} ${OSGLUA_PUBLIC_API})
set_target_properties(osgLua_static PROPERTIES PUBLIC_HEADER "${OSGLUA_PUBLIC_API}")
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
