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

add_definitions(-DOSGLUA_LIBRARY_STATIC)

add_definitions(-DLUABIND_MAX_BASES=6)
add_definitions(-DLUABIND_MAX_ARITY=3)

set(SOURCES
	bindUtils/WrapperClass_Macros.h
	Internal_PositionInterface.cpp
	Internal_PositionInterface.h
	Internal_DigitalInterface.cpp
	Internal_DigitalInterface.h
	LuaScript.cpp
	BindKernelToLua.h
	BindOsgToLua.h
	BindOsgAppToLua.h
	BindGadgetInterfacesToLua.h)

set(API
	LuaScript.h)


set(LUA_BINDING_COMPONENTS
	BindKernelToLua.cpp
	BindOsgToLua.cpp
	BindOsgAppToLua.cpp
	BindGadgetInterfacesToLua.cpp)

if(NOT BUILD_LUABIND_COMBINED)
	list(APPEND SOURCES ${LUA_BINDING_COMPONENTS})
endif()

add_library(vrjugglua ${SOURCES} ${API})
set_target_properties(vrjugglua PROPERTIES PUBLIC_HEADER "${API}")
target_link_libraries(vrjugglua ${EXTRA_LIBS})
stamp_target_with_version("${CPACK_PACKAGE_VERSION}" vrjugglua)
add_cppcheck(vrjugglua)
