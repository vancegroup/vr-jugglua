# Embedded build for osgLua
# 2009-2010 Ryan Pavlik <rpavlik@iastate.edu>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

# Must configure your top-level project to find osgIntrospection

include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})

include_directories(${LUA_INCLUDE_DIRS})

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

add_subdirectory(src/osgLua)
