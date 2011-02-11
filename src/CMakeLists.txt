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

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${VRJUGGLER_INCLUDE_DIRS})

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
	"vrjlua Standalone Interpreter")

set_target_properties(vrjlua PROPERTIES FOLDER "Applications")

create_target_launcher(vrjlua
	FORWARD_ARGS
	RUNTIME_LIBRARY_DIRS
	${RUNTIME_LIBRARY_DIRS}
    WORKING_DIRECTORY
    "${PROJECT_BINARY_DIR}"
	ENVIRONMENT
	${VRJUGGLER_ENVIRONMENT})

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
		"Nav Testbed FLTK")

	set_target_properties(${FLTK_GUI_APP} PROPERTIES FOLDER "Applications")

    install(TARGETS ${FLTK_GUI_APP}
	    RUNTIME DESTINATION ${BIN_DIR} COMPONENT runtime
		BUNDLE DESTINATION . COMPONENT runtime)
endif()

###
# Qt-based GUIs
###
if(BUILD_QT_GUI)
	include(${QT_USE_FILE})

	set(BUILD_WITHOUT_TERMINAL OFF)
	set(QT_EXTRA_LIBS)
	set(QT_EXE_OPTIONS)
	if(WIN32)
		if(QT_QTMAIN_LIBRARY AND NOT BUILD_VERBOSE)
			set(BUILD_WITHOUT_TERMINAL ON)
			set(QT_EXTRA_LIBS ${QT_QTMAIN_LIBRARY})
			set(QT_EXE_OPTIONS WIN32)
		endif()
	else()
		set(BUILD_WITHOUT_TERMINAL ON)
	endif()

	if(BUILD_WITHOUT_TERMINAL)
		add_definitions(-DBUILD_WITHOUT_TERMINAL)
	endif()

    add_executable(NavTestbed MACOSX_BUNDLE ${QT_EXE_OPTIONS} QTNavMain.cpp)
    target_link_libraries(NavTestbed vrjugglua-qt ${QT_EXTRA_LIBS})

	set_property(TARGET
		NavTestbed
		PROPERTY
		PROJECT_LABEL
		"Nav Testbed Qt")
	set_target_properties(NavTestbed PROPERTIES FOLDER "Applications")

    install(TARGETS NavTestbed
	    RUNTIME DESTINATION ${BIN_DIR} COMPONENT runtime
		BUNDLE DESTINATION . COMPONENT runtime)
endif()


###
# Set up and provide installation commands (needed for packaging to work)
###

include("BundleOSGPlugins")

install_osg_plugins(OSG_PLUGINS)

if(APPLE AND TARGET NavTestbed)
	install_vrjuggler_data_files(./NavTestbed.app/Contents)
	install_vrjuggler_plugins(./NavTestbed.app/Contents VRJUGGLER_INSTALLED_PLUGINS)
	set(APP_LOCATION ".")
else()
	install_vrjuggler_data_files(.)
	install_vrjuggler_plugins(. VRJUGGLER_INSTALLED_PLUGINS)
	set(APP_LOCATION "bin")
endif()

if(TARGET NavTestbed)
	fixup_vrjuggler_app_bundle("NavTestbed" "${APP_LOCATION}" "${RUNTIME_LIBRARIES};${VRJUGGLER_INSTALLED_PLUGINS};${OSG_PLUGINS}" "${RUNTIME_LIBRARY_DIRS}")

	create_target_launcher(NavTestbed
		FORWARD_ARGS
		RUNTIME_LIBRARY_DIRS
		${RUNTIME_LIBRARY_DIRS}
		WORKING_DIRECTORY
		"${PROJECT_BINARY_DIR}"
		ENVIRONMENT
		${VRJUGGLER_ENVIRONMENT})
endif()
