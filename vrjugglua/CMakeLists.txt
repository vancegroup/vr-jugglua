# CMake cross-platform build system
# 2009-2010 Ryan Pavlik <rpavlik@iastate.edu>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

if(BUILD_LUABIND_COMBINED)
	add_definitions(-DLUABIND_COMBINED_COMPILE)
endif()

foreach(wrapper OSG OSGDB OSGUTIL)
	if(AUTOLOAD_WRAPPER_${wrapper})
		add_definitions(-DAUTOLOAD_WRAPPER_${wrapper})
	endif()
endforeach()

if(BUILD_EMBEDDED_LUA)
	add_definitions(-DEMBEDDED_LUA)
endif()

add_definitions(-DOSGLUA_LIBRARY_STATIC)

include_directories(${VRJUGGLER22_INCLUDE_DIRS})

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
	VRJLua_C_Interface.cpp)

set(API
	ConvertOsgPtrToOsgLuaPtr.h
	Internal_AnalogInterface.h
	Internal_DigitalInterface.h
	Internal_PositionInterface.h
	LuaConsole.h
	LuaIncludeFull.h
	LuaInclude.h
	LuaPath.h
	LuaRunBuffer.h
	LuaScript.h
	OsgAppProxy.h
	SynchronizedRunBuffer.h
	VRJLua_C_Interface.h
	VRJLuaOutput.h
	"${CMAKE_CURRENT_BINARY_DIR}/VRJLuaConfig.h")

set(LUA_BINDING_SOURCES
	binding_detail/BindKernelToLua.h	
	binding_detail/BindOsgToLua.h
	binding_detail/BindSonixToLua.h
	binding_detail/BindGadgetInterfacesToLua.h
	binding_detail/BindRunBufferToLua.h
	binding_detail/OsgHelpers.h
	binding_detail/OsgHelpers.cpp)

set(LUA_BINDING_IMPLEMENTATIONS
	binding_detail/BindKernelToLua.cpp
	binding_detail/BindOsgToLua.cpp
	binding_detail/BindSonixToLua.cpp
	binding_detail/BindGadgetInterfacesToLua.cpp
	binding_detail/BindRunBufferToLua.cpp)

if(NOT BUILD_LUABIND_COMBINED)
	list(APPEND LUA_BINDING_SOURCES ${LUA_BINDING_IMPLEMENTATIONS})
endif()

source_group("Binding Details" FILES ${LUA_BINDING_SOURCES})

# If we have an older boost, work around it
if(Boost_VERSION LESS 103500)
	include_directories(bitsOfBoost143)
	if(INSTALL_CXX_SDK)
		install(DIRECTORY bitsOfBoost143/boost
			DESTINATION ${INCLUDE_DIR})
	endif()
endif()

add_library(vrjugglua ${SOURCES} ${LUA_BINDING_SOURCES} ${API})
set_target_properties(vrjugglua PROPERTIES PUBLIC_HEADER "${API}")
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
	#add_subdirectory(qt-console)
endif()

if(BUILD_FLTK_GUI)
	add_subdirectory(fltk-console)
endif()
