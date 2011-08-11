# Ryan Pavlik <rpavlik@iastate.edu>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

include(LuaTargets)

set(LUA_FILES
	help.lua)

add_lua_target(lua-docstring "${CMAKE_BINARY_DIR}/${LUA_DIR}" help.lua)

set_property(TARGET
	lua-docstring
	PROPERTY
	PROJECT_LABEL
	"Lua help module")

# Install into the install location, as well
install_lua_target(lua-docstring
	DESTINATION
	${LUA_DIR}
	COMPONENT
	runtime)
add_dependencies(lua_modules lua-docstring)