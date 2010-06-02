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

set(LUA_FILES ${LUA_FILES} PARENT_SCOPE)

add_custom_target(luaFiles ALL)
add_custom_command(TARGET luaFiles
			POST_BUILD
			COMMAND
			${CMAKE_COMMAND}
			ARGS -E make_directory "${CMAKE_BINARY_DIR}/${LUA_DIR}"
			COMMENT "Creating directory ${CMAKE_BINARY_DIR}/${LUA_DIR}")

foreach(luafile ${LUA_FILES})
	add_custom_command(TARGET luaFiles
			POST_BUILD
			COMMAND
			${CMAKE_COMMAND}
			ARGS -E copy "${luafile}" "${CMAKE_BINARY_DIR}/${LUA_DIR}"
			WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
			DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${luafile}"
			COMMENT "Copying ${luafile} to ${CMAKE_BINARY_DIR}/${LUA_DIR}")
endforeach()

install(FILES
	${LUA_FILES}
	DESTINATION
	${LUA_DIR})
