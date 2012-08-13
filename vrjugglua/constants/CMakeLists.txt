set(outname "gldef.lua")
set(script "${CMAKE_CURRENT_SOURCE_DIR}/extract.lua")
set(input "${CMAKE_CURRENT_SOURCE_DIR}/gl.h")
set(tablename gldef)

set(outfile "${CMAKE_BINARY_DIR}/${LUA_DIR}/${outname}")

add_custom_command(OUTPUT "${outfile}"
	COMMAND "${LUA_INTERPRETER}" "${script}" "${input}" ${tablename} > "${outfile}"
	DEPENDS "${script}" "${input}"
	COMMENT "Extracting OpenGL constants to table '${tablename}' in ${outname}"
	VERBATIM)

add_custom_target(extract_gl_constants ALL
	DEPENDS "${outfile}"
	SOURCES "${script}")

install(FILES "${outfile}"
	DESTINATION ${LUA_DIR}
	COMPONENT runtime)

