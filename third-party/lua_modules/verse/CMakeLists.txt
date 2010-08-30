add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/verse.lua"
	COMMAND ${SQUISH_COMMAND} --output="${CMAKE_CURRENT_BINARY_DIR}/verse.lua" "${CMAKE_CURRENT_SOURCE_DIR}")
	
add_custom_target(verse ALL DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/verse.lua")
add_dependencies(verse squish)