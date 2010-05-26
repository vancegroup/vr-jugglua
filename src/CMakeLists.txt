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


###
# vrj::OsgApp Lua Delegation
###
set(APPDELEGATION_SOURCES
	LuaOSGApp.cpp
	LuaOSGApp.h
	main.cpp)
add_executable(vrjlua-delegation ${APPDELEGATION_SOURCES})
target_link_libraries(vrjlua-delegation vrjugglua)
stamp_target_with_version("${CPACK_PACKAGE_VERSION}" vrjlua-delegation)
add_cppcheck(vrjlua-delegation)


###
# vrj lua app object launcher
###
set(APPOBJ_SOURCES
	runLuaAppObject.cpp)
add_executable(vrjlua-appobjectlauncher ${APPOBJ_SOURCES})
target_link_libraries(vrjlua-appobjectlauncher vrjugglua)
stamp_target_with_version("${CPACK_PACKAGE_VERSION}" vrjlua-appobjectlauncher)
add_cppcheck(vrjlua-appobjectlauncher)

###
# Standard Lua interpreter with VRJ and OSG automatically loaded
###
add_executable(vrjlua lua.c)
target_link_libraries(vrjlua vrjugglua)



