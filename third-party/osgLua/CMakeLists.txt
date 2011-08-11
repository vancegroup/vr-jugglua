# Embedded build for osgLua
# Ryan Pavlik <rpavlik@iastate.edu>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

# Must configure your top-level project to find osgIntrospection

include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})

include_directories(${LUA_INCLUDE_DIRS})

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories("${CMAKE_CURRENT_BINARY_DIR}/include")

if(OSGLUA_USE_CPPINTROSPECTION)
	include_directories(${CPPINTROSPECTION_INCLUDE_DIRS})
	set(INTROSPECTION_LIBRARIES ${CPPINTROSPECTION_LIBRARIES})
endif()

add_subdirectory(src/osgLua)
