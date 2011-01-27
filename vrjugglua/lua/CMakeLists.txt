# CMake cross-platform build system
# 2009-2010 Ryan Pavlik <rpavlik@iastate.edu>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

include(LuaTargets)

set(LUA_FILES
	Actions.lua
	getScriptFilename.lua
	Navigator.lua
	nav-testbed-launcher.lua
	osgnav-testbed.lua
	osgTools.lua
	Scene.lua
	sleep.lua
	StateMachine.lua
	vrjlua-config.lua
	vrjlua-init.lua)


add_lua_target(luaFiles "${CMAKE_BINARY_DIR}/${LUA_DIR}" ${LUA_FILES})

set_property(TARGET
	luaFiles
	PROPERTY
	PROJECT_LABEL
	"VRJLua Lua Scripts")

# Install into the install location, as well
install_lua_target(luaFiles
	DESTINATION
	${LUA_DIR}
	COMPONENT
	runtime)
