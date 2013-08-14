# Embedded build for osgLua
# Ryan Pavlik <rpavlik@iastate.edu>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

# Must configure your top-level project to find osgIntrospection

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/generate/lua-CodeGen/src/CodeGen.lua")
	message(FATAL_ERROR "Can't build: missing the lua-CodeGen submodule. Try running git submodule update --init --recursive to get all submodules.")
endif()

include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})

include_directories(${LUA_INCLUDE_DIRS})

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories("${CMAKE_CURRENT_BINARY_DIR}/include")
include_directories("${CMAKE_CURRENT_BINARY_DIR}/generate")

if(OSGLUA_USE_CPPINTROSPECTION)
	include_directories(${CPPINTROSPECTION_INCLUDE_DIRS})
	set(INTROSPECTION_LIBRARIES ${CPPINTROSPECTION_LIBRARIES})
endif()

add_subdirectory(generate)
add_subdirectory(src/osgLua)
