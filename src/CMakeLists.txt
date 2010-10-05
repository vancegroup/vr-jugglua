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
if(MSVC)
    # Silence warning from Lua distribution's code
    set_property(TARGET
        vrjlua
        PROPERTY
        COMPILE_DEFINITIONS
        _CRT_SECURE_NO_WARNINGS)
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
	if(NOT BUILD_QT_GUI)
		set(FLTK_GUI_APP NavTestbed)
	else()
		set(FLTK_GUI_APP FLTKNavTestbed)
	endif()
    add_executable(${FLTK_GUI_APP} MACOSX_BUNDLE FLTKNavMain.cpp)
    target_link_libraries(${FLTK_GUI_APP} vrjugglua-fltk)
	
	set_property(TARGET
		${FLTK_GUI_APP}
		PROPERTY
		PROJECT_LABEL
		"App: Nav Console FLTK")

    install(TARGETS ${FLTK_GUI_APP}
	    RUNTIME DESTINATION ${BIN_DIR} COMPONENT runtime
		BUNDLE DESTINATION . COMPONENT runtime)
endif()

###
# FLTK-based GUIs
###
if(BUILD_QT_GUI)
	include(${QT_USE_FILE})
	
    add_executable(NavTestbed MACOSX_BUNDLE QTNavMain.cpp)
    target_link_libraries(NavTestbed vrjugglua-qt)
	
	set_property(TARGET
		NavTestbed
		PROPERTY
		PROJECT_LABEL
		"App: Nav Console QT")

    install(TARGETS NavTestbed
	    RUNTIME DESTINATION ${BIN_DIR} COMPONENT runtime
		BUNDLE DESTINATION . COMPONENT runtime)
endif()


###
# Set up and provide installation commands (needed for packaging to work)
###

include("BundleOSGPlugins")

#install_osg_plugins(OSG_PLUGINS)

if(APPLE AND TARGET NavTestbed)
	install_vrjuggler22_data_files(./NavTestbed.app)
	install_vrjuggler22_gadgeteer_drivers(./NavTestbed.app VRJUGGLER22_INSTALLED_PLUGINS)
else()
	install_vrjuggler22_data_files(.)
	install_vrjuggler22_gadgeteer_drivers(. VRJUGGLER22_INSTALLED_PLUGINS)
endif()

fixup_vrjuggler_app_bundle("NavTestbed" "bin" "${RUNTIME_LIBRARIES};${VRJUGGLER22_INSTALLED_PLUGINS};${OSGDB_PLUGINS_RELEASE};${OSGWRAPPER_PLUGINS_RELEASE}" "${RUNTIME_LIBRARY_DIRS}")
