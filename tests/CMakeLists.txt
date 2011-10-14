if(BUILD_LUA_AS_CPP)
	add_definitions(-DLUABIND_CPLUSPLUS_LUA)
endif()

add_definitions(-DOSGLUA_LIBRARY_STATIC)

add_definitions(${LUABIND_DEFINITIONS})

include_directories(${VRJUGGLER_INCLUDE_DIRS}
	${OPENSCENEGRAPH_INCLUDE_DIRS}
	${LUABIND_INCLUDE_DIRS}
	${OSGLUA_INCLUDE_DIRS}
)

set(TESTS_DATA "${CMAKE_CURRENT_SOURCE_DIR}/data")
set(LUA_TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/luatests")

set(LUA_TEST_SCRIPTS
	actions-loads
	actions-canremove
	stop-kernel
	minimal-osg
	initialize-devices
	navtestbed-runs
	osgnav-statemachine
	osgapp-delegate
	osgapp-delegate-count
	osgapp-delegate-count-frameactions
	osgapp-delegate-useful
	osgapp-delegate-initdevices
	osgapp-delegate-devicedata
	osg-accept-visitor
	osglua-allwrappers
	osglua-basicwrappers
	osglua-dotnotcolon
	osglua-enums
	osglua-matrixmath
	osglua-missingmethod
	osglua-props
	osglua-usedisplaylistprop
	osglua-vec3array
	osglua-vectormath)

foreach(script ${LUA_TEST_SCRIPTS})
    add_test(NAME
        vrjlua-run-${script}_lua
        COMMAND
        ${GENERIC_LAUNCHER_COMMAND}
        --debugger
        $<TARGET_FILE:vrjlua>
        "${LUA_TEST_DIR}/${script}.lua")
endforeach()

set(EXAMPLE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../examples")
file(GLOB_RECURSE EXAMPLES
	RELATIVE "${EXAMPLE_DIR}"
	"${EXAMPLE_DIR}/*.lua")
foreach(EXAMPLE ${EXAMPLES})
	if("${EXAMPLE}" MATCHES "^[0-9]+-")
		string(REGEX REPLACE "[.\\/]" "_" script "${EXAMPLE}")
		add_test(NAME
			examples_${script}
			COMMAND
			${GENERIC_LAUNCHER_COMMAND}
			--debugger
			$<TARGET_FILE:NavTestbed>
			--stub
			"${EXAMPLE_DIR}/${EXAMPLE}"
			"${CMAKE_CURRENT_SOURCE_DIR}/shutdown-after-5-frames.lua")
	endif()
endforeach()

add_boost_test(OsgPointers
	SOURCES
	OsgPointers.cpp
	LIBRARIES
	vrjugglua
	TESTS
	RefExactMatch
	RefSingleConversion
	RefMultipleConversion
	RefInvalidConversion
	RefInvalidRefConversion
	RefPtrExactMatch
	RefPtrSingleConversion
	RefPtrMultipleConversion
	RefPtrInvalidConversion
	RefPtrInvalidRefConversion
	ValExactMatch
	ValSingleConversion
	ValSingleConversionFromFloat
	ValSingleConversionFromDouble
	ValInvalidConversion
	ValInvalidValConversion
	MatrixExactMatch
	MatrixSingleConversion
	MatrixSingleConversionFromFloat # osgIntrospection doesn't convert from matrixf to matrixd
	MatrixExactMatchFromRef	#Conversion from refmatrix to matrix not supported?
	MatrixSingleConversionFromRef
	MatrixSingleConversionFromFloatRef
)

add_boost_test(OsgNumericConversion
	SOURCES
	OsgNumericConversion.cpp
	LIBRARIES
	vrjugglua
	TESTS
	ConversionPathVec3fToVec3d
	ConstructionVec3fToVec3d
	Vec3fToVec3dCompatibleSet
	Vec3fToVec3dUsingSet
	GetConversionVec3fToVec3d
)
