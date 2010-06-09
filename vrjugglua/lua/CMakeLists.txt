# CMake cross-platform build system
# 2009-2010 Ryan Pavlik <rpavlik@iastate.edu>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

set(LUA_FILES
	Navigator.lua
	osgnav-testbed.lua
	osgTools.lua
	StateMachine.lua
	vrjlua-config.lua)

# Custom command to create the lua directory
add_custom_command(OUTPUT "${CMAKE_BINARY_DIR}/${LUA_DIR}"
			COMMAND
			${CMAKE_COMMAND}
			ARGS -E make_directory "${CMAKE_BINARY_DIR}/${LUA_DIR}"
			COMMENT "Creating directory ${CMAKE_BINARY_DIR}/${LUA_DIR}")

# Custom commands to install Lua files
set(ALLFILES)
foreach(luafile ${LUA_FILES})
	add_custom_command(OUTPUT "${CMAKE_BINARY_DIR}/${LUA_DIR}/${luafile}"
			COMMAND
			${CMAKE_COMMAND}
			ARGS -E copy "${luafile}" "${CMAKE_BINARY_DIR}/${LUA_DIR}"
			WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
			DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${luafile}" "${CMAKE_BINARY_DIR}/${LUA_DIR}"
			COMMENT "Copying ${luafile} to ${CMAKE_BINARY_DIR}/${LUA_DIR}")
	list(APPEND ALLFILES "${CMAKE_BINARY_DIR}/${LUA_DIR}/${luafile}")
endforeach()

# Custom target depending on all the lua file commands
add_custom_target(luaFiles
		SOURCES ${LUA_FILES}
		DEPENDS ${ALLFILES}
		COMMENT "Lua files successfully installed into build tree.")

set_property(TARGET
		luaFiles
		PROPERTY
		PROJECT_LABEL
		"VRJLua Lua Scripts")

# Install into the install location, as well
install(FILES
	${LUA_FILES}
	DESTINATION
	${LUA_DIR})
