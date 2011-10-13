# Embedded build for osgLua
# Ryan Pavlik <rpavlik@iastate.edu>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

set(OSGLUA_SRCS
	ArrayPropertyProxy.cpp
	BindValue.cpp
	Callback.cpp
	CustomConverters.cpp
	loadObjectFile.cpp
	lua_functions.cpp
	MathValueTags.cpp
	MatrixTemplates.h
	osgLua.cpp
	loadWrapperLib.cpp
	loadWrapperLib.h
	saveObjectFile.cpp
	Script.cpp
	Type.cpp
	Value.cpp
	Value_rawget.inl
	Value_metamethod_members.cpp
	Value_metamethods.cpp
	Value_metamethods.h
	VectorTemplates.h)

set(OSGLUA_PRIVATE_API
	ArrayPropertyProxy.h
	CustomConverters.h
	lua_functions.h
	LuaIncludeFull.h
	LuaUserdata.h
	MathValueTags.h
	osgLua.h
	Type.h)

set(HEADER_DIR ../../include/osgLua)
set(OSGLUA_PUBLIC_API
	${HEADER_DIR}/Callback
	${HEADER_DIR}/LoadWrapper
	${HEADER_DIR}/LuaInclude
	${HEADER_DIR}/Script
	${HEADER_DIR}/Value
	"${CMAKE_CURRENT_BINARY_DIR}/${HEADER_DIR}/Config")
include(${HEADER_DIR}/introspection/headers.cmake)
source_group(API FILES ${OSGLUA_PUBLIC_API})

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${HEADER_DIR}/")
configure_file("${HEADER_DIR}/Config.in" "${CMAKE_CURRENT_BINARY_DIR}/${HEADER_DIR}/Config")

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
target_link_libraries(osgLua_static lua ${OPENSCENEGRAPH_LIBRARIES} ${INTROSPECTION_LIBRARIES} lua)

set_property(TARGET
	osgLua_static
	PROPERTY
	PROJECT_LABEL
	"OsgLua Static Library")

set_target_properties(osgLua_static PROPERTIES FOLDER "Third-Party")

if(INSTALL_CXX_SDK)
	install(TARGETS osgLua_static
		EXPORT vrjlua-sdk
		RUNTIME DESTINATION ${BIN_DIR} COMPONENT runtime
		LIBRARY DESTINATION ${LIB_DIR} COMPONENT runtime
		ARCHIVE DESTINATION ${ARCH_DIR} COMPONENT sdk
		PUBLIC_HEADER DESTINATION ${INCLUDE_DIR}/osgLua COMPONENT sdk)
	install(FILES
		${OSGLUA_INTROSPECTION_HEADERS}
		DESTINATION ${INCLUDE_DIR}/osgLua/introspection COMPONENT sdk)
endif()
