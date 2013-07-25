set(OUTFILE "${CMAKE_CURRENT_BINARY_DIR}/RegisterMathMetamethods.inl")
set(SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/generate-operators.lua")

if(NOT LUA_INTERPRETER)
	if(TARGET lua-interp)
		set(LUA_INTERPRETER lua-interp)
		set(LUA_DEP lua-interp)
	else()
		
	endif()
endif()

message(STATUS "${LUA_INTERPRETER} ${SCRIPT} > ${OUTFILE}")
add_custom_command(OUTPUT "${OUTFILE}"
	COMMAND "${LUA_INTERPRETER}" "${SCRIPT}" > "${OUTFILE}"
	DEPENDS "${SCRIPT}" "mathtypehelpers.lua" "operators-template.lua"
	WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
	COMMENT "Generating math operator metamethods"
	
)

add_custom_target(osgLua-generate-operators
	SOURCES "${OUTFILE}"
	DEPENDS ${LUA_DEP})
