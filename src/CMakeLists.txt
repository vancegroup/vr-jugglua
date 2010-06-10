# CMake cross-platform build system
# 2009-2010 Ryan Pavlik <rpavlik@iastate.edu>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

if(BUILD_LUABIND_COMBINED)
	add_definitions(-DLUABIND_COMBINED_COMPILE)
endif()

if(BUILD_EMBEDDED_LUA)
	add_definitions(-DEMBEDDED_LUA)
endif()

add_definitions(-DOSGLUA_LIBRARY_STATIC)

if(${Boost_VERSION} LESS 103500)
	include_directories(../vrjugglua/bitsOfBoost143)
endif()

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${VRJUGGLER22_INCLUDE_DIRS})

###
# Standard Lua interpreter with VRJ and OSG automatically loaded
###
add_executable(vrjlua lua.c)
target_link_libraries(vrjlua vrjugglua)
if(BUILD_LUA_AS_CPP)
	set_source_files_properties(lua.c PROPERTIES LANGUAGE CXX)
endif()
set_property(TARGET
	vrjlua
	PROPERTY
	PROJECT_LABEL
	"App: Standalone Interpreter")

install(TARGETS vrjlua
	RUNTIME DESTINATION ${BIN_DIR} COMPONENT runtime)



###
# FLTK-based GUIs
###
if(BUILD_FLTK_GUI)	
    add_executable(NavTestbed FLTKNavMain.cpp)
    target_link_libraries(NavTestbed vrjugglua-fltk)
	
	set_property(TARGET
		NavTestbed
		PROPERTY
		PROJECT_LABEL
		"App: Nav Console FLTK")

    install(TARGETS NavTestbed
	    RUNTIME DESTINATION ${BIN_DIR})
endif()
