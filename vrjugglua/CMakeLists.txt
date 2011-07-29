# CMake cross-platform build system
# 2009-2010 Ryan Pavlik <rpavlik@iastate.edu>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

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

include_directories(${VRJUGGLER_INCLUDE_DIRS})

###
# Let the Lua files install themselves

add_subdirectory(lua)

set(SOURCES
	Internal_AnalogInterface.cpp
	Internal_DigitalInterface.cpp
	Internal_PositionInterface.cpp
	LuaConsole.cpp
	LuaRunBuffer.cpp
	LuaScript.cpp
	LuaPath.cpp
	OsgAppProxy.cpp
	SynchronizedRunBuffer.cpp
	VRJLua_C_Interface.cpp
	VRJLuaConfig.h.in)

set(API
	osgLuaBind.h
	Internal_AnalogInterface.h
	Internal_DigitalInterface.h
	Internal_PositionInterface.h
	LuaConsole.h
	LuaGCBlock.h
	LuaIncludeFull.h
	LuaInclude.h
	LuaPath.h
	LuaRunBuffer.h
	LuaScript.h
	OsgAppProxy.h
	SynchronizedRunBuffer.h
	VRJLua_C_Interface.h
	VRJLuaOutput.h)

set(GENERATED
	"${CMAKE_CURRENT_BINARY_DIR}/VRJLuaConfig.h")

source_group("Generated Files - Do Not Edit" FILES ${GENERATED})

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

add_library(vrjugglua ${SOURCES} ${LUA_BINDING_SOURCES} ${API} ${GENERATED})
set_target_properties(vrjugglua PROPERTIES PUBLIC_HEADER "${API};${GENERATED}")
target_link_libraries(vrjugglua ${EXTRA_LIBS})
stamp_target_with_version("${CPACK_PACKAGE_VERSION}" vrjugglua)
add_cppcheck(vrjugglua)

set_property(TARGET
	vrjugglua
	PROPERTY
	PROJECT_LABEL
	"VRJLua main library")

# Require the Lua files installed into the build tree
add_dependencies(vrjugglua luaFiles)

if(INSTALL_CXX_SDK)
	install(TARGETS vrjugglua
		EXPORT vrjlua-sdk
		RUNTIME DESTINATION ${BIN_DIR} COMPONENT runtime
		LIBRARY DESTINATION	${LIB_DIR} COMPONENT runtime
		ARCHIVE DESTINATION	${ARCH_DIR} COMPONENT sdk
		PUBLIC_HEADER DESTINATION ${INCLUDE_DIR}/vrjugglua COMPONENT sdk)
endif()

###
# Optional GUI components

if(BUILD_QT_GUI)
	add_subdirectory(qt-console)
endif()

if(BUILD_FLTK_GUI)
	add_subdirectory(fltk-console)
endif()
