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
set(CPACK_PACKAGE_VERSION_PATCH "4")
set(CPACK_PACKAGE_VERSION
	"${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME}-${CPACK_PACKAGE_VERSION}-src")

###
# Set up options
###

# Define Simple Options
option(BUILD_LUABIND_COMBINED "Compile all luabind wrappers in a single CPP rather than multiple - shorter full rebuild time, but longer incremental build time." OFF)
option(BUILD_VERBOSE "Turn on lots of runtime output." off)
option(BUILD_FLTK_GUI "Build FLTK-based GUI apps." on)
option(BUILD_QT_GUI "Build Qt-based GUI apps." off)

option(INSTALL_CXX_SDK "Should we install the headers and libraries needed for C++ development using VRJLua?" on)

option(AUTOLOAD_WRAPPER_OSG "Load the osg wrapper automatically at binding time." ON)
option(AUTOLOAD_WRAPPER_OSGDB "Load the osgDB wrapper automatically at binding time." OFF)
option(AUTOLOAD_WRAPPER_OSGUTIL "Load the osgUtil wrapper automatically at binding time." OFF)

# Define advanced options
option(BUILD_EMBEDDED_LUA "Build Lua, LuaBind, and osgLua as a part of the app - highly recommended." ON)
option(BUILD_SHARED_OSGLUA "Whether to also build a DLL/.so version of osgLua." OFF)
option(BUILD_LUA_AS_CPP "Build Lua as C++, rather than C - recommended." ON)
option(BUILD_VERY_VERBOSE "Turn on excessive amounts of runtime output." OFF)

# Mark some as advanced
mark_as_advanced(BUILD_EMBEDDED_LUA
	BUILD_SHARED_OSGLUA
	BUILD_LUA_AS_CPP
	BUILD_VERY_VERBOSE)

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
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(UseBackportedModules)
include(CppcheckTargets)
include(DoxygenTargets)
include(GetDirectoryList)
include(EnableExtraCompilerWarnings)
include(StampSourcesWithVersion)

include(MSVCMultipleProcessCompile)

include(CTest)
include(CreateDashboardScripts)

set(EXTRA_LIBS)

set(CMAKE_DEBUG_POSTFIX  "_d")

# Boost
find_package(Boost 1.34.0 REQUIRED)
if(WIN32)
	foreach(lib FILESYSTEM SYSTEM)
		get_filename_component(_libname "${Boost_${lib}_LIBRARY_RELEASE}" NAME_WE)
		get_filename_component(_libpath "${Boost_${lib}_LIBRARY_RELEASE}" PATH)
		list(APPEND CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS "${_libpath}/${_libname}.dll")
	endforeach()
endif()

# OpenSceneGraph
include(SearchProgramFilesForOpenSceneGraph)
find_package(OpenSceneGraph REQUIRED COMPONENTS osgDB osgUtil osgIntrospection)
list(APPEND EXTRA_LIBS ${OPENSCENEGRAPH_LIBRARIES})

include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})
get_directory_list(OPENSCENEGRAPH_RUNTIME_LIBRARY_DIRS ${OPENSCENEGRAPH_LIBRARIES})
list(APPEND RUNTIME_LIBRARY_DIRS ${OPENSCENEGRAPH_RUNTIME_LIBRARY_DIRS})

# VR Juggler
find_package(VRJuggler22 COMPONENTS VRJOGL22 SONIX12 REQUIRED)

add_definitions(${VRJUGGLER22_DEFINITIONS})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${VRJUGGLER22_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${VRJUGGLER22_CXX_FLAGS}")
list(APPEND EXTRA_LIBS ${VRJUGGLER22_LIBRARIES})

include(BundleOSGRuntime)
include(BundleVRJ22Runtime)

# Lua
if(BUILD_EMBEDDED_LUA)
	add_definitions(-DLUABIND_MAX_BASES=3)
	add_definitions(-DLUABIND_MAX_ARITY=3)
else()
	find_package(Luabind REQUIRED)
	list(APPEND EXTRA_LIBS ${LUABIND_LIBRARIES})
	include_directories(${LUABIND_INCLUDE_DIRS})
	add_definitions(${LUABIND_DEFINITIONS})

	find_package(osgLua REQUIRED)
	list(APPEND EXTRA_LIBS ${OSGLUA_LIBRARIES})
	include_directories(${OSGLUA_INCLUDE_DIRS})
endif()

# FLTK
if(BUILD_FLTK_GUI)
	set(CMAKE_FIND_APPBUNDLE NEVER) # Don't find Fluid.app the single-site browser
	find_package(FLTK REQUIRED)
endif()

# Qt
if(BUILD_QT_GUI)
	find_package(Qt4 COMPONENTS QtCore QtGui Qt3Support REQUIRED)
endif()

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/vrjugglua/VRJLuaConfig.h.in"
	"${CMAKE_CURRENT_BINARY_DIR}/vrjugglua/VRJLuaConfig.h")

include_directories("${CMAKE_CURRENT_BINARY_DIR}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

###
# Build the project
###

# Build the embedded libraries
add_subdirectory(third-party)

if(BUILD_EMBEDDED_LUA)
	list(APPEND EXTRA_LIBS luabind osgLua_static)
	include_directories(${LUABIND_INCLUDE_DIRS} ${OSGLUA_INCLUDE_DIRS})
endif()

if(BUILD_FLTK_GUI)
	set(FLTK_GUI_LIBS ${FNFC_LIBRARY})
	include_directories(${FNFC_INCLUDE_DIRS} ${FLTK_INCLUDE_DIR})
endif()

# Build the vrjugglua library
add_subdirectory(vrjugglua)

# Build the applications
add_subdirectory(src)

add_doxygen(Doxyfile)

if(BUILD_TESTING)
	include(BoostTestTargets)
	add_subdirectory(tests)
endif()

create_dashboard_scripts("DashboardBuildInitialCache.cmake.in")


###
# Create config files to allow easy use of this library
###

# Set up use from build tree
set(BUILD_TREE_TARGETS lua osgLua_static luabind vrjugglua)
if(BUILD_FLTK_GUI)
	list(APPEND BUILD_TREE_TARGETS Fl_Native_File_Chooser vrjugglua-fltk)
endif()

configure_file(vrjugglua-config-build-tree.cmake.in VRJuggLuaConfig.cmake @ONLY)
export(TARGETS ${BUILD_TREE_TARGETS}
	NAMESPACE vrjlua_buildtree_
	FILE vrjugglua-targets.cmake)
export(PACKAGE VRJuggLua)

# Set up use from install tree
if(INSTALL_CXX_SDK)
	install(EXPORT vrjlua-sdk
		DESTINATION ${EXPORT_DIR}
		NAMESPACE vrjlua_exported_
		FILE vrjlua-targets.cmake)
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
