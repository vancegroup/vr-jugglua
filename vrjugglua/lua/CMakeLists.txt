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

install(FILES
	${LUA_FILES}
	DESTINATION
	${LUA_DIR})
