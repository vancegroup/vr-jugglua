set(outname "gldef.lua")
set(script "${CMAKE_CURRENT_SOURCE_DIR}/extract.lua")
set(input "${CMAKE_CURRENT_SOURCE_DIR}/gl.h")
set(tablename gldef)

if(NOT LUA_INTERPRETER)
	if(TARGET lua-interp)
		set(LUA_INTERPRETER lua-interp)
		set(LUA_DEP lua-interp)
	endif()
endif()

set(outfile "${CMAKE_BINARY_DIR}/${LUA_DIR}/${outname}")

add_custom_command(OUTPUT "${outfile}"
	COMMAND "${LUA_INTERPRETER}" "${script}" "${input}" ${tablename} > "${outfile}"
	DEPENDS "${script}" "${input}"
	COMMENT "Extracting OpenGL constants to table '${tablename}' in ${outname}"
	VERBATIM)

add_custom_target(extract_gl_constants ALL
	DEPENDS "${outfile}"
	SOURCES "${script}")

if(LUA_DEP)
	add_dependencies(extract_gl_constants ${LUA_DEP})
endif()

install(FILES "${outfile}"
	DESTINATION ${LUA_DIR}
	COMPONENT runtime)

