# CMake cross-platform build system
# Ryan Pavlik <rpavlik@iastate.edu>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

include(LuaTargets)

set(LUA_FILES
	base.lua
	debug_ext.lua
	getopt.lua
	io_ext.lua
	lcs.lua
	list.lua
	math_ext.lua
    mbox.lua
    modules.lua
    object.lua
    parser.lua
    set.lua
    std.lua
    strict.lua
    string_ext.lua
    table_ext.lua
    tree.lua
    xml.lua)


add_lua_target(stdlib "${CMAKE_BINARY_DIR}/${LUA_DIR}" ${LUA_FILES})

set_property(TARGET
	stdlib
	PROPERTY
	PROJECT_LABEL
	"Lua stdlib modules")

# Install into the install location, as well
install_lua_target(stdlib
	DESTINATION
	${LUA_DIR}
	COMPONENT
	runtime)
