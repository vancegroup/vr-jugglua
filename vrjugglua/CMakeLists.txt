# CMake cross-platform build system
# 2009-2010 Ryan Pavlik <rpavlik@iastate.edu>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

set(SOURCES
	Lua_PositionInterface.cpp
	LuaScript.cpp)

set(API
	LuaScript.h)

set(APP_SOURCES
	LuaOSGApp.cpp
	main.cpp)

add_library(vrjugglua ${SOURCES} ${API})
set_target_properties(vrjugglua PROPERTIES PUBLIC_HEADER "${API}")
target_link_libraries(vrjugglua ${EXTRA_LIBS})
stamp_target_with_version("${CPACK_PACKAGE_VERSION}" vrjugglua)
add_cppcheck(vrjugglua)


add_executable(vrjugglua-launcher ${APP_SOURCES})
target_link_libraries(vrjugglua-launcher vrjugglua)
stamp_target_with_version("${CPACK_PACKAGE_VERSION}" vrjugglua-launcher)
add_cppcheck(vrjugglua-launcher)
