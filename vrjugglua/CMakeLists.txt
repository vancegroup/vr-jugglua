if(BUILD_LUABIND_COMBINED)
	add_definitions(-DLUABIND_COMBINED_COMPILE)
endif()

if(BUILD_EMBEDDED_LUA)
	add_definitions(-DEMBEDDED_LUA)
endif()

if(BUILD_LUA_AS_CPP)
	add_definitions(-DLUABIND_CPLUSPLUS_LUA)
endif()

add_definitions(-DOSGLUA_LIBRARY_STATIC)

add_definitions(${LUABIND_DEFINITIONS})

include_directories(${VRJUGGLER_INCLUDE_DIRS}
	${OPENSCENEGRAPH_INCLUDE_DIRS}
	${LUABIND_INCLUDE_DIRS}
	${OSGLUA_INCLUDE_DIRS}
)

###
# Let the Lua files install themselves

add_subdirectory(lua)
add_subdirectory(constants)

set(SOURCES
	FindVPRDLL.cpp
	FindVPRDLL.h
	Internal_AnalogInterface.cpp
	Internal_DigitalInterface.cpp
	Internal_PositionInterface.cpp
	Internal_StringInterface.cpp
	LuaConsole.cpp
	LuaRunBuffer.cpp
	LuaScript.cpp
	LuaPath.cpp
	OsgAppProxy.cpp
	Reconfiguration.cpp
	SearchPath.cpp
	SynchronizedRunBuffer.cpp
	VRJLua_C_Interface.cpp
	VRJLuaConfig.h.in)

set(API
	osgLuaBind.h
	Internal_AnalogInterface.h
	Internal_DigitalInterface.h
	Internal_PositionInterface.h
	Internal_StringInterface.h
	LuaConsole.h
	LuaGCBlock.h
	LuaIncludeFull.h
	LuaInclude.h
	LuaPath.h
	LuaRunBuffer.h
	LuaScript.h
	OsgAppProxy.h
	Reconfiguration.h
	SearchPath.h
	SynchronizedRunBuffer.h
	VRJLua_C_Interface.h
	VRJLuaOutput.h)

set(LUA_BINDING_SOURCES
	binding_detail/BindKernelToLua.h
	binding_detail/BindOsgToLua.h
	binding_detail/BindSonixToLua.h
	binding_detail/BindGadgetInterfacesToLua.h
	binding_detail/BindRunBufferToLua.h
	binding_detail/BindvrjLuaToLua.h
	binding_detail/BindKernelToLua.inl
	binding_detail/BindOsgToLua.inl
	binding_detail/BindSonixToLua.inl
	binding_detail/BindGadgetInterfacesToLua.inl
	binding_detail/BindRunBufferToLua.inl
	binding_detail/BindvrjLuaToLua.inl)

source_group("Binding Details" FILES ${LUA_BINDING_SOURCES})

add_library(vrjugglua ${SOURCES} ${LUA_BINDING_SOURCES} ${API})
set_target_properties(vrjugglua PROPERTIES PUBLIC_HEADER "${API}")
target_link_libraries(vrjugglua ${VRJUGGLER_LIBRARIES} ${OPENSCENEGRAPH_LIBRARIES} ${LUABIND_LIBRARIES} ${OSGLUA_LIBRARIES})

add_cppcheck(vrjugglua)

set_property(TARGET
	vrjugglua
	PROPERTY
	PROJECT_LABEL
	"VRJLua main library")

# Require the Lua files installed into the build tree
add_dependencies(vrjugglua luaFiles extract_gl_constants)

if(INSTALL_CXX_SDK)
	install(TARGETS vrjugglua
		EXPORT vrjlua-sdk
		RUNTIME DESTINATION ${BIN_DIR} COMPONENT runtime
		LIBRARY DESTINATION	${LIB_DIR} COMPONENT runtime
		ARCHIVE DESTINATION	${ARCH_DIR} COMPONENT sdk
		PUBLIC_HEADER DESTINATION ${INCLUDE_DIR}/vrjugglua COMPONENT sdk)
	install(FILES
		"${CMAKE_CURRENT_BINARY_DIR}/VRJLuaConfig.h"
		DESTINATION ${INCLUDE_DIR}/vrjugglua COMPONENT sdk)
endif()

###
# Optional GUI components

if(BUILD_QT_GUI)
	add_subdirectory(qt-console)
endif()

if(BUILD_FLTK_GUI)
	add_subdirectory(fltk-console)
endif()
