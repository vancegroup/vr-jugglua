MACRO(OSGLUA_EXAMPLE EXEC_NAME)

	file(GLOB EXEC_SOURCES "${osgLua_SOURCE_DIR}/examples/${EXEC_NAME}/*.cpp")

	add_executable(${EXEC_NAME}
		${EXEC_SOURCES}
	)

	target_link_libraries(${EXEC_NAME}
			osgLua
	)

	set(libs ${ARGV})
	list(REMOVE_AT libs 0)
	list(LENGTH libs libs_len)

	if(libs_len)
		target_link_libraries(${EXEC_NAME} ${libs})
	endif(libs_len)


ENDMACRO(OSGLUA_EXAMPLE EXEC_NAME)

include_directories(
	${osgLua_SOURCE_DIR}/include
	${OSG_INCLUDE_DIR}
	${LUA_INCLUDE_DIR}
)

OSGLUA_EXAMPLE(osgluaconsole osgText osgViewer)
OSGLUA_EXAMPLE(osgluainterpreter osgDB)


