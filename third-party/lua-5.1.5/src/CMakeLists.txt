# Embedded build for Lua 5.1

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
	set(LUA_EXTRA_DEFINES _CRT_SECURE_NO_DEPRECATE CACHE INTERNAL "" FORCE)
elseif(APPLE)
	set(LUA_EXTRA_DEFINES LUA_USE_MACOSX CACHE INTERNAL "" FORCE)
else() # Linux
	find_library(READLINE_LIBRARY
		NAMES
		readline)
	find_package(Curses)
	if(READLINE_LIBRARY AND CURSES_FOUND)
		list(APPEND LUA_EXTRA_LIBS ${READLINE_LIBRARY} ${CURSES_LIBRARIES})
		set(LUA_EXTRA_DEFINES LUA_USE_LINUX CACHE INTERNAL "" FORCE)
	else()
		set(LUA_EXTRA_DEFINES LUA_USE_POSIX LUA_USE_DLOPEN CACHE INTERNAL "" FORCE)
	endif()
endif()

if(CMAKE_DL_LIBS)
	list(APPEND LUA_EXTRA_LIBS ${CMAKE_DL_LIBS})
endif()

set(LUA_EXTRA_LIBS ${LUA_EXTRA_LIBS} CACHE INTERNAL "" FORCE)

if(BUILD_SHARED_LUA)
	set(LIBTYPE SHARED)
	set(LUALIB_EXTRA_DEFINES LUA_BUILD_AS_DLL)
else()
	set(LIBTYPE STATIC)
	set(LUALIB_EXTRA_DEFINES)
endif()

add_library(lua ${LIBTYPE} ${LUA_SRCS} ${LUA_HEADERS} ${LUA_API})

if(NOT MSVC)
	list(APPEND LUA_EXTRA_LIBS m)
endif()

if(LUA_EXTRA_LIBS)
	target_link_libraries(lua ${LUA_EXTRA_LIBS})
endif()

set_property(TARGET
    lua
    PROPERTY
    COMPILE_DEFINITIONS
    ${LUA_EXTRA_DEFINES} ${LUALIB_EXTRA_DEFINES})

if(BUILD_SHARED_LUA)
	set_property(TARGET
		lua
		PROPERTY
		OUTPUT_NAME
		lua51)
endif()

if(BUILD_LUA_UTILS)
	add_executable(lua-interp ${LUA_INTERP_SOURCES})
	target_link_libraries(lua-interp lua)
	set_target_properties(lua-interp PROPERTIES FOLDER "Third-Party")
	set_property(TARGET
		lua-interp
		PROPERTY
		COMPILE_DEFINITIONS
		${LUA_EXTRA_DEFINES})

	if(BUILD_SHARED_LUA)
		# Essentially statically link luac
		list(APPEND LUAC_SOURCES ${LUA_SRCS} ${LUA_HEADERS} ${LUA_API})
	endif()

	add_executable(luac ${LUAC_SOURCES})

	if(BUILD_SHARED_LUA)
		if(LUA_EXTRA_LIBS)
			target_link_libraries(luac ${LUA_EXTRA_LIBS})
		endif()
	else()
		target_link_libraries(luac lua)
	endif()

	set_target_properties(luac PROPERTIES FOLDER "Third-Party")

	set_property(TARGET
		luac
		PROPERTY
		COMPILE_DEFINITIONS
		${LUA_EXTRA_DEFINES})
	install(TARGETS luac
		EXPORT vrjlua-luac
		RUNTIME DESTINATION ${BIN_DIR} COMPONENT runtime)
endif()

if(BUILD_LUA_AS_CPP)
	set_source_files_properties(${LUA_SRCS} ${LUA_INTERP_SOURCES} ${LUAC_SOURCES} PROPERTIES LANGUAGE CXX)
endif()

set_property(TARGET
	lua
	PROPERTY
	PROJECT_LABEL
	"Lua Core Library")
set_target_properties(lua PROPERTIES PUBLIC_HEADER "${LUA_API}")
set_target_properties(lua PROPERTIES FOLDER "Third-Party")

if(INSTALL_CXX_SDK)
	install(TARGETS lua
		EXPORT vrjlua-sdk
		RUNTIME DESTINATION ${BIN_DIR} COMPONENT runtime
		LIBRARY DESTINATION ${LIB_DIR} COMPONENT runtime
		ARCHIVE DESTINATION ${ARCH_DIR} COMPONENT sdk
		PUBLIC_HEADER DESTINATION ${INCLUDE_DIR} COMPONENT sdk)
endif()
