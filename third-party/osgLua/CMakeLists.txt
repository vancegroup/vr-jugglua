project(osgLua)

option(OSGLUA_EXAMPLES "Compile osgLua Examples" YES)
option(OSGLUA_USE_INTERNAL_LUA "Compile lua inside osgLuas" YES)

set(CMAKE_MODULE_PATH "${osgLua_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")
set(LIBRARY_OUTPUT_PATH ${osgLua_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${osgLua_SOURCE_DIR}/bin)
link_directories(${LIBRARY_OUTPUT_PATH})

find_package(OSG REQUIRED)

if (NOT OSGLUA_USE_INTERNAL_LUA)
	find_package(Lua REQUIRED)
else (NOT OSGLUA_USE_INTERNAL_LUA)
	set(LUA_INCLUDE_DIR "${osgLua_SOURCE_DIR}/src/lua-5.1.2/src")
	set(LUA_LIBRARY "")
endif(NOT OSGLUA_USE_INTERNAL_LUA)


add_subdirectory(src)
if(OSGLUA_EXAMPLES)
	add_subdirectory(examples)
endif(OSGLUA_EXAMPLES)

