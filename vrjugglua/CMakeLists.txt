# CMake cross-platform build system
# 2009-2010 Ryan Pavlik <rpavlik@iastate.edu>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

if(BUILD_VERBOSE)
	add_definitions(-DVERBOSE)
endif()

if(BUILD_LUABIND_COMBINED)
	add_definitions(-DLUABIND_COMBINED_COMPILE)
endif()

if(BUILD_EMBEDDED_LUA)
	add_definitions(-DEMBEDDED_LUA)
endif()

add_definitions(-DLUABIND_MAX_BASES=6)
add_definitions(-DLUABIND_MAX_ARITY=3)

set(SOURCES
	bindUtils/WrapperClass_Macros.h
	Internal_PositionInterface.cpp
	Internal_PositionInterface.h
	LuaScript.cpp
	BindKernelToLua.h
	BindOsgToLua.h
	BindOsgAppToLua.h
	BindPositionInterfaceToLua.h)

set(API
	LuaScript.h)


set(LUA_BINDING_COMPONENTS
	BindKernelToLua.cpp
	BindOsgToLua.cpp
	BindOsgAppToLua.cpp
	BindPositionInterfaceToLua.cpp)

if(NOT BUILD_LUABIND_COMBINED)
	list(APPEND SOURCES ${LUA_BINDING_COMPONENTS})
endif()


set(APP_SOURCES
	LuaOSGApp.cpp
	LuaOSGApp.h
	main.cpp)

set(LUAAPPOBJ_SOURCES
	runLuaAppObject.cpp)

add_library(vrjugglua ${SOURCES} ${API})
set_target_properties(vrjugglua PROPERTIES PUBLIC_HEADER "${API}")
target_link_libraries(vrjugglua ${EXTRA_LIBS})
stamp_target_with_version("${CPACK_PACKAGE_VERSION}" vrjugglua)
add_cppcheck(vrjugglua)


add_executable(vrjugglua-launcher ${APP_SOURCES})
target_link_libraries(vrjugglua-launcher vrjugglua)
stamp_target_with_version("${CPACK_PACKAGE_VERSION}" vrjugglua-launcher)
add_cppcheck(vrjugglua-launcher)

add_executable(vrjugglua-appobject ${LUAAPPOBJ_SOURCES})
target_link_libraries(vrjugglua-appobject vrjugglua)
stamp_target_with_version("${CPACK_PACKAGE_VERSION}" vrjugglua-appobject)
add_cppcheck(vrjugglua-appobject)
