# CMake cross-platform build system
# 2009-2010 Ryan Pavlik <rpavlik@iastate.edu>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

cmake_minimum_required(VERSION 2.6.2)

# Set package properties
project(VRJuggLua)

set(CPACK_PACKAGE_VENDOR "Iowa State University")
set(CPACK_PACKAGE_CONTACT "Ryan Pavlik <rpavlik@iastate.edu>")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "5")
set(CPACK_PACKAGE_VERSION
	"${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME}-${CPACK_PACKAGE_VERSION}-src")

###
# Set up options
###

if(APPLE AND CMAKE_SIZEOF_VOID_P EQUAL 8)
	# FLTK on Mac OS X 64-bit doesn't exist
	set(HIDE_FLTK TRUE)
endif()

# Set up build directories
if(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
endif()
if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
endif()
if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()
if(NOT SHARE_OUTPUT_DIRECTORY)
	set(SHARE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/share)
endif()
if(NOT ROOT_OUTPUT_DIRECTORY)
	set(ROOT_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

# Define Simple Options
option(BUILD_DOCS "Add a target to build documentation" on)
option(BUILD_VERBOSE "Turn on lots of runtime output." off)
option(BUILD_WITH_VRJ30 "Use VR Juggler 3.0 instead of 2.2" on)
if(NOT HIDE_FLTK)
	option(BUILD_FLTK_GUI "Build FLTK-based GUI apps." off)
endif()
option(BUILD_QT_GUI "Build Qt-based GUI apps." on)

option(INSTALL_CXX_SDK "Should we install the headers and libraries needed for C++ development using VRJLua?" on)

if(WIN32 OR APPLE)
	set(SNAPSHOT_ROOT_README_DEFAULT on)
	set(BUNDLE_DEFAULT on)
	set(BUILD_WITHOUT_TERMINAL_DEFAULT ON)
else()
	set(SNAPSHOT_ROOT_README_DEFAULT off)
	set(BUNDLE_DEFAULT off)
	set(BUILD_WITHOUT_TERMINAL_DEFAULT OFF)
endif()

option(INSTALL_SNAPSHOT_README_TO_ROOT "Should we install the binary snapshot readme to the root install directory?" ${SNAPSHOT_ROOT_README_DEFAULT})

option(BUILD_WITHOUT_TERMINAL "Build GUI applications without the terminal when possible." ${BUILD_WITHOUT_TERMINAL_DEFAULT})

option(BUILD_WITH_CPPINTROSPECTION "Build against cppintrospection instead of osgIntrospection." off)

option(INSTALL_LIBRARIES_BUNDLE "Use BundleUtils to install dependent libraries, etc." ${BUNDLE_DEFAULT})

# Define advanced options
option(BUILD_EMBEDDED_LUA "Build Lua, LuaBind, and osgLua as a part of the app - highly recommended." ON)
option(BUILD_LUA_UTILS "Build a stock, standalone lua-interpreter and luac" ON)
option(BUILD_SHARED_OSGLUA "Whether to also build a DLL/.so version of osgLua." OFF)
option(BUILD_SHARED_LUA "Whether to build Lua as a shared library." OFF)
option(BUILD_SHARED_LUABIND "Whether to build LuaBind as a shared library." OFF)
option(BUILD_LUA_AS_CPP "Build Lua as C++, rather than C - recommended." OFF)
option(BUILD_VERY_VERBOSE "Turn on excessive amounts of runtime output." OFF)

# Mark some as advanced
mark_as_advanced(BUILD_EMBEDDED_LUA
	BUILD_LUA_UTILS
	BUILD_SHARED_OSGLUA
	BUILD_SHARED_LUA
	BUILD_SHARED_LUABIND
	BUILD_LUA_AS_CPP
	BUILD_VERY_VERBOSE)

if(MSVC)
	option(BUILD_WITH_PROJECT_FOLDERS
		"Use project folders in build system - not compatible with Visual C++ Express editions!"
		OFF)
else()
	set(BUILD_WITH_PROJECT_FOLDERS ON)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ${BUILD_WITH_PROJECT_FOLDERS})

# Apply definitions
if(BUILD_VERBOSE)
	set(VERBOSE ON)
endif()

if(BUILD_VERY_VERBOSE)
	set(VERY_VERBOSE ON)
endif()

###
# Perform build configuration of dependencies
###

set(BIN_DIR bin)
set(INCLUDE_DIR include)
set(SHARE_DIR share/vrjugglua/)
set(LUA_DIR share/vrjugglua/lua/)
set(ARCH_DIR lib)
if(WIN32)
	set(LIB_DIR bin)
	set(EXPORT_DIR cmake)
else()
	set(LIB_DIR lib)
	set(EXPORT_DIR ${LIB_DIR}/VRJuggLua)
endif()

# Locally-developed modules dist'ed with this app - always have this first.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(UseBackportedModules)
include(CppcheckTargets)
include(DoxygenTargets)
include(GetDirectoryList)
include(EnableExtraCompilerWarnings)
include(StampSourcesWithVersion)
include(ResetConfigurations) # remove Debug and MinSizeRel

include(MSVCMultipleProcessCompile)
include(CreateLaunchers)

include(CTest)
include(CreateDashboardScripts)

include(SetDefaultBuildType)
set_default_build_type(RelWithDebInfo)

set(EXTRA_LIBS)

set(CMAKE_DEBUG_POSTFIX  "_d")

# Boost
# program_options added for the VR Juggler 3.0 compatibility work
find_package(Boost 1.35.0 REQUIRED filesystem system signals program_options)
list(APPEND EXTRA_LIBS ${Boost_LIBRARIES})
if(WIN32)
	foreach(lib FILESYSTEM SYSTEM)
		get_filename_component(_libname "${Boost_${lib}_LIBRARY_RELEASE}" NAME_WE)
		get_filename_component(_libpath "${Boost_${lib}_LIBRARY_RELEASE}" PATH)
		list(APPEND CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS "${_libpath}/${_libname}.dll")
	endforeach()
endif()

# OpenSceneGraph
if(NOT BUILD_WITH_CPPINTROSPECTION)
	set(OSGINTROSPECTION_COMPONENT osgIntrospection)
endif()
include(SearchProgramFilesForOpenSceneGraph)
find_package(OpenSceneGraph REQUIRED COMPONENTS osgDB osgUtil ${OSGINTROSPECTION_COMPONENT})

if(APPLE)
	# TODO: Make this workaround prettier or, better, fix the libgif issue
	# Someone is using the old libgif in ImageIO.framework
	# Therefore we need to add its path to the runtime library dirs
	list(APPEND RUNTIME_LIBRARY_DIRS "/System/Library/Frameworks/ApplicationServices.framework/Versions/A/Frameworks/ImageIO.framework/Versions/A/Resources")
endif()

get_directory_list(OPENSCENEGRAPH_RUNTIME_LIBRARY_DIRS ${OPENSCENEGRAPH_LIBRARIES})
list(APPEND RUNTIME_LIBRARY_DIRS ${OPENSCENEGRAPH_RUNTIME_LIBRARY_DIRS})
if(WIN32)
    foreach(_dir ${OPENSCENEGRAPH_RUNTIME_LIBRARY_DIRS})
        list(APPEND RUNTIME_LIBRARY_DIRS ${_dir}/../bin)
    endforeach()
endif()


if(BUILD_WITH_CPPINTROSPECTION)
	find_package(cppintrospection)
	list(APPEND OPENSCENEGRAPH_LIBRARIES ${CPPINTROSPECTION_LIBRARIES})
	list(APPEND OPENSCENEGRAPH_INCLUDE_DIRS ${CPPINTROSPECTION_INCLUDE_DIRS})
endif()

# VR Juggler
if(BUILD_WITH_VRJ30)
	find_package(VRJuggler 3.0 EXACT REQUIRED)
else()
	find_package(VRJuggler 2.2 EXACT REQUIRED)
endif()

add_definitions(${VRJUGGLER_DEFINITIONS})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${VRJUGGLER_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${VRJUGGLER_CXX_FLAGS}")
list(APPEND EXTRA_LIBS ${VRJUGGLER_LIBRARIES})
list(APPEND RUNTIME_LIBRARY_DIRS ${VRJUGGLER_RUNTIME_LIBRARY_DIRS})

# Lua
if(BUILD_EMBEDDED_LUA)
	set(LUABIND_DEFINITIONS -DLUABIND_MAX_BASES=3 -DLUABIND_MAX_ARITY=3)
	set(LUABIND_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/third-party/luabind")
	set(LUABIND_LIBRARIES luabind)
	if(BUILD_SHARED_LUABIND)
		list(APPEND LUABIND_DEFINITIONS "-DLUABIND_DYNAMIC_LINK")
	endif()

	set(OSGLUA_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/third-party/osgLua")
	set(OSGLUA_LIBRARIES osgLua)
else()
	find_package(Luabind REQUIRED)

	find_package(osgLua REQUIRED)
endif()

# FLTK
if(BUILD_FLTK_GUI)
	set(CMAKE_FIND_APPBUNDLE NEVER) # Don't find Fluid.app the single-site browser
	find_package(FLTK REQUIRED)
	if(NOT FLTK_FLUID_EXECUTABLE)
		message(FATAL_ERROR "Could not find the FLTK Fluid UI design tool!")
	endif()
endif()

# Qt
if(BUILD_QT_GUI)
	find_package(Qt4 COMPONENTS QtCore QtGui REQUIRED)
	if(WIN32)
		list(APPEND RUNTIME_LIBRARY_DIRS ${QT_LIBRARY_DIR}/../bin)
	else()
		list(APPEND RUNTIME_LIBRARY_DIRS ${QT_LIBRARY_DIR})
	endif()
endif()

if(NOT (WIN32 OR APPLE))
	find_library(READLINE_LIBRARY
		NAMES
		readline)
	find_package(Curses)
	if(READLINE_LIBRARY AND CURSES_FOUND)
		set(LUA_USE_LINUX 1)
	else()
		set(LUA_USE_POSIX 1)
		set(LUA_USE_DLOPEN 1)
	endif()
endif()

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/vrjugglua/VRJLuaConfig.h.in"
	"${CMAKE_CURRENT_BINARY_DIR}/vrjugglua/VRJLuaConfig.h")

include_directories("${CMAKE_CURRENT_BINARY_DIR}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

# For generating documentation in HTML
if(WIN32)
	list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/extra/discount-2.1.2")
endif()
find_package(Markdown)
if(MARKDOWN_FOUND)
	include(UseMarkdown)
endif()

###
# Build the project
###

# Build the embedded libraries
add_subdirectory(third-party)

if(BUILD_EMBEDDED_LUA)
	list(APPEND EXTRA_LIBS ${LUABIND_LIBRARIES} ${OSGLUA_LIBRARIES})
endif()

if(BUILD_FLTK_GUI)
	set(FLTK_GUI_LIBS ${FNFC_LIBRARY})
endif()

# Add a flag for indicating the source tree location
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/${LUA_DIR}")
file(WRITE "${CMAKE_BINARY_DIR}/${LUA_DIR}/vrjlua-sourcetreeloc.lua" "${CMAKE_SOURCE_DIR}")

# Build the vrjugglua library
add_subdirectory(vrjugglua)

# Build the applications
add_subdirectory(src)

# Build documentation
add_subdirectory(doc)
if(BUILD_DOCS)
	add_doxygen(Doxyfile)
endif()

if(APPLE)
	string(REPLACE "/usr/local/lib;" "" RUNTIME_LIBRARY_DIRS "${RUNTIME_LIBRARY_DIRS}")
	string(REPLACE "/usr/lib;" "" RUNTIME_LIBRARY_DIRS "${RUNTIME_LIBRARY_DIRS}")
endif()

create_default_target_launcher(NavTestbed
	FORWARD_ARGS
	RUNTIME_LIBRARY_DIRS
	${RUNTIME_LIBRARY_DIRS}
    WORKING_DIRECTORY
    "${CMAKE_CURRENT_SOURCE_DIR}"
	ENVIRONMENT
	${VRJUGGLER_ENVIRONMENT})

create_generic_launcher(genericlauncher
	RUNTIME_LIBRARY_DIRS
	${RUNTIME_LIBRARY_DIRS}
    WORKING_DIRECTORY
    "${CMAKE_CURRENT_SOURCE_DIR}"
	ENVIRONMENT
	${VRJUGGLER_ENVIRONMENT})


if(BUILD_TESTING)
	include(BoostTestTargets)
	add_subdirectory(tests)
endif()

create_dashboard_scripts("DashboardBuildInitialCache.cmake.in")


###
# Create config files to allow easy use of this library
###

# Set up use from build tree
set(BUILD_TREE_TARGETS lua osgLua luabind vrjugglua)
if(BUILD_FLTK_GUI)
	list(APPEND BUILD_TREE_TARGETS Fl_Native_File_Chooser vrjugglua-fltk)
endif()
if(BUILD_QT_GUI)
	list(APPEND BUILD_TREE_TARGETS vrjugglua-qt)
endif()

configure_file(vrjugglua-config-build-tree.cmake.in VRJuggLuaConfig.cmake @ONLY)
export(TARGETS ${BUILD_TREE_TARGETS}
	NAMESPACE vrjlua_buildtree_
	FILE vrjugglua-targets.cmake)
if(BUILD_LUA_UTILS)
	export(TARGETS luac
		NAMESPACE
		FILE vrjugglua-luac-target.cmake)
endif()
export(PACKAGE VRJuggLua)


# Set up use from install tree
if(INSTALL_CXX_SDK)
	install(EXPORT vrjlua-sdk
		DESTINATION ${EXPORT_DIR}
		NAMESPACE vrjlua_exported_
		FILE vrjlua-targets.cmake)
	install(EXPORT vrjlua-luac
		DESTINATION ${EXPORT_DIR}
		NAMESPACE
		FILE vrjlua-luac-target.cmake)
	configure_file(vrjugglua-config.cmake.in vrjugglua-config-for-install.cmake @ONLY)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/vrjugglua-config-for-install.cmake
		RENAME vrjugglua-config.cmake
		DESTINATION ${EXPORT_DIR})
endif()

###
# Set packaging options (for CPack)
###

# Install the examples
install(DIRECTORY examples
	DESTINATION . COMPONENT examples)

# Install assets (Fonts, etc)
install(DIRECTORY assets
	DESTINATION ${SHARE_DIR} COMPONENT recommendedassets)

include(InstallRequiredSystemLibraries)

# Choose desired package generators
if(APPLE)
	set(CPACK_GENERATOR DragNDrop)
	set(CPACK_SOURCE_GENERATOR ZIP)
elseif(WIN32)
	set(CPACK_SOURCE_GENERATOR ZIP)
else()
	set(CPACK_SOURCE_GENERATOR TARGZ)
endif()

# Include the packaging system now that we have it all set up
include(CPack)

###
# End Packaging
###
