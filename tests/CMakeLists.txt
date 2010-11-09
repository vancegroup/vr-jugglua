# CMake cross-platform build system
# 2009-2010 Ryan Pavlik <rpavlik@iastate.edu>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

set(TESTS_DATA "${CMAKE_CURRENT_SOURCE_DIR}/data")
set(LUA_TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/luatests")

set(LUA_TEST_SCRIPTS
	stop-kernel
	minimal-osg
	initialize-devices
	osgnav-statemachine
	osgapp-delegate
	osgapp-delegate-count
	osgapp-delegate-useful
	osgapp-delegate-initdevices
	osgapp-delegate-devicedata
	osg-accept-visitor)

foreach(script ${LUA_TEST_SCRIPTS})
    add_test(NAME
        vrjlua-run-${script}_lua
        COMMAND
        ${GENERIC_LAUNCHER_COMMAND}
        --debugger
        $<TARGET_FILE:vrjlua>
        "${LUA_TEST_DIR}/${script}.lua")
endforeach()

add_boost_test(OsgPointers
	SOURCES
	OsgPointers.cpp
	LIBRARIES
	vrjugglua)
