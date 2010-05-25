# Embedded build for LuaBind
# 2009-2010 Ryan Pavlik <rpavlik@iastate.edu>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

include_directories("${Boost_INCLUDE_DIRS}")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
include_directories("${LUA_INCLUDE_DIRS}")
add_subdirectory(src)
