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
	InternalConfig.h
	lua_functions.h
	LuaIncludeFull.h
	LuaUserdata.h
	LuaStackChecker.h
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

if(BUILD_LUA_AS_CPP)
	add_definitions(-DOSGLUA_LUA_BUILT_AS_CPP)
endif()

if(BUILD_SHARED_OSGLUA)
	set(LIBTYPE SHARED)
else()
	set(LIBTYPE STATIC)
	set(OSGLUA_LIBRARY_STATIC)
	add_definitions(-DOSGLUA_LIBRARY_STATIC)
endif()

configure_file("${HEADER_DIR}/Config.in" "${CMAKE_CURRENT_BINARY_DIR}/${HEADER_DIR}/Config")


add_library(osgLua ${LIBTYPE} ${OSGLUA_SRCS} ${OSGLUA_PRIVATE_API} ${OSGLUA_PUBLIC_API})
set_target_properties(osgLua PROPERTIES PUBLIC_HEADER "${OSGLUA_PUBLIC_API}")
target_link_libraries(osgLua lua ${OPENSCENEGRAPH_LIBRARIES} ${INTROSPECTION_LIBRARIES} ${OPENGL_LIBRARIES} lua)

set_property(TARGET
	osgLua
	PROPERTY
	PROJECT_LABEL
	"osgLua Library")

set_target_properties(osgLua PROPERTIES FOLDER "Third-Party")

if(INSTALL_CXX_SDK)
	install(TARGETS osgLua
		EXPORT vrjlua-sdk
		RUNTIME DESTINATION ${BIN_DIR} COMPONENT runtime
		LIBRARY DESTINATION ${LIB_DIR} COMPONENT runtime
		ARCHIVE DESTINATION ${ARCH_DIR} COMPONENT sdk
		PUBLIC_HEADER DESTINATION ${INCLUDE_DIR}/osgLua COMPONENT sdk)
	install(FILES
		${OSGLUA_INTROSPECTION_HEADERS}
		DESTINATION ${INCLUDE_DIR}/osgLua/introspection COMPONENT sdk)
endif()
