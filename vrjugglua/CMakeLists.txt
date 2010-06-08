# CMake cross-platform build system
# 2009-2010 Ryan Pavlik <rpavlik@iastate.edu>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

if(BUILD_LUABIND_COMBINED)
	add_definitions(-DLUABIND_COMBINED_COMPILE)
endif()

foreach(wrapper OSG OSGDB OSGUTIL)
	if(AUTOLOAD_WRAPPER_${wrapper})
		add_definitions(-DAUTOLOAD_WRAPPER_${wrapper})
	endif()
endforeach()

if(BUILD_EMBEDDED_LUA)
	add_definitions(-DEMBEDDED_LUA)
endif()

add_definitions(-DOSGLUA_LIBRARY_STATIC)

set(SOURCES
	Internal_AnalogInterface.cpp
	Internal_DigitalInterface.cpp
	Internal_PositionInterface.cpp
	LuaRunBuffer.cpp
	LuaScript.cpp
	LuaPath.cpp
	OsgAppProxy.cpp
	SynchronizedRunBuffer.cpp
	VRJLua_C_Interface.cpp)

set(API
	Internal_AnalogInterface.h
	Internal_DigitalInterface.h
	Internal_PositionInterface.h
	BindKernelToLua.h
	BindOsgToLua.h
	BindSonixToLua.h
	BindGadgetInterfacesToLua.h
	BindRunBufferToLua.h
	LuaInclude.h
	LuaRunBuffer.h
	LuaScript.h
	LuaPath.h
	OsgAppProxy.h
	SynchronizedRunBuffer.h
	VRJLua_C_Interface.h)

set(LUA_BINDING_COMPONENTS
	BindKernelToLua.cpp
	BindOsgToLua.cpp
	BindSonixToLua.cpp
	BindGadgetInterfacesToLua.cpp
	BindRunBufferToLua.cpp)

if(NOT BUILD_LUABIND_COMBINED)
	list(APPEND SOURCES ${LUA_BINDING_COMPONENTS})
endif()

# If we have an older boost, work around it
if(${Boost_VERSION} LESS 103500)
	include_directories(bitsOfBoost143)
	install(DIRECTORY bitsOfBoost143/boost
		DESTINATION ${INCLUDE_DIR})
endif()

add_library(vrjugglua ${SOURCES} ${API})
set_target_properties(vrjugglua PROPERTIES PUBLIC_HEADER "${API}")
target_link_libraries(vrjugglua ${EXTRA_LIBS})
stamp_target_with_version("${CPACK_PACKAGE_VERSION}" vrjugglua)
add_cppcheck(vrjugglua)

set_property(TARGET
	vrjugglua
	PROPERTY
	PROJECT_LABEL
	"VR JuggLua library")

install(TARGETS vrjugglua
	EXPORT vrjlua-sdk
	RUNTIME DESTINATION ${BIN_DIR}
	ARCHIVE DESTINATION	${LIB_DIR}
	PUBLIC_HEADER DESTINATION ${INCLUDE_DIR}/vrjugglua)

###
# Let the Lua files install themselves

add_subdirectory(lua)
