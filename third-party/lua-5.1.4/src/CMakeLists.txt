# Embedded build for Lua 5.1
# 2009-2010 Ryan Pavlik <rpavlik@iastate.edu>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

set(LUA_SRCS
	lapi.c
	lauxlib.c
	lbaselib.c
	lcode.c
	ldblib.c
	ldebug.c
	ldo.c
	ldump.c
	lfunc.c
	lgc.c
	linit.c
	liolib.c
	llex.c
	lmathlib.c
	lmem.c
	loadlib.c
	lobject.c
	lopcodes.c
	loslib.c
	lparser.c
	lstate.c
	lstring.c
	lstrlib.c
	ltable.c
	ltablib.c
	ltm.c
	#lua.c	# not needed for embedded version
	#luac.c	# not needed for embedded version
	lundump.c
	lvm.c
	lzio.c
	print.c)
set(LUA_HEADERS
	lapi.h
	lcode.h
	ldebug.h
	ldo.h
	lfunc.h
	lgc.h
	llex.h
	llimits.h
	lmem.h
	lobject.h
	lopcodes.h
	lparser.h
	lstate.h
	lstring.h
	ltable.h
	ltm.h
	lundump.h
	lvm.h
	lzio.h)
set(LUA_API
	luaconf.h
	lua.h
	lualib.h
	lauxlib.h)
set(LUA_INTERP_SOURCES
	lua.c)
set(LUAC_SOURCES
	luac.c)

set(LUA_EXTRA_LIBS)
if(WIN32)
	add_definitions(/D_CRT_SECURE_NO_DEPRECATE)
elseif(APPLE)
	add_definitions(-DLUA_USE_MACOSX)
else() # Linux
	add_definitions(-DLUA_USE_LINUX)

	find_library(READLINE_LIBRARY
		NAMES
		readline)
	find_package(Curses)

	list(APPEND LUA_EXTRA_LIBS ${READLINE_LIBRARY} ${CURSES_LIBRARIES})
endif()

add_library(lua ${LUA_SRCS} ${LUA_HEADERS} ${LUA_API})
if(LUA_EXTRA_LIBS)
	target_link_libraries(lua ${LUA_EXTRA_LIBS})
endif()

if(BUILD_LUA_UTILS)
	add_executable(lua-interp ${LUA_INTERP_SOURCES})
	target_link_libraries(lua-interp lua)

	add_executable(luac ${LUAC_SOURCES})
	target_link_libraries(luac lua)

	install(TARGETS luac
		EXPORT vrjlua-sdk
		RUNTIME DESTINATION ${BIN_DIR} COMPONENT runtime)
endif()

if(BUILD_LUA_AS_CPP)
	set_source_files_properties(${LUA_SRCS} ${LUA_INTERP_SOURCES} ${LUAC_SOURCES} PROPERTIES LANGUAGE CXX)
endif()

set_property(TARGET
	lua
	PROPERTY
	PROJECT_LABEL
	"Third-Party: Lua Core Library")
set_target_properties(lua PROPERTIES PUBLIC_HEADER "${LUA_API}")

if(INSTALL_CXX_SDK)
	install(TARGETS lua
		EXPORT vrjlua-sdk
		RUNTIME DESTINATION ${BIN_DIR} COMPONENT runtime
		LIBRARY DESTINATION ${LIB_DIR} COMPONENT runtime
		ARCHIVE DESTINATION ${ARCH_DIR} COMPONENT sdk
		PUBLIC_HEADER DESTINATION ${INCLUDE_DIR} COMPONENT sdk)
endif()
