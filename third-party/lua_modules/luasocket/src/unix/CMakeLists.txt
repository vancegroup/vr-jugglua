# Ugh. I'm very irritated that the Luasocket people rename the modules to "core". (No prefix, .dll/.so suffix). This makes life difficult because I can't build the libraries directly with the name because there are two libraries (socket, mime) so the names conflict with each other.
# Furthermore, CMake doesn't offer good support for renaming TARGETS on install.
# The easiest way to work around this problem was create a new subdirectory just for the mime plugin and build directly to the name 'core'.

SET(luasocket_UNIX
		${LuaSocket_SOURCE_DIR}/src/buffer.h
		${LuaSocket_SOURCE_DIR}/src/buffer.c
		${LuaSocket_SOURCE_DIR}/src/auxiliar.h
		${LuaSocket_SOURCE_DIR}/src/auxiliar.c
		${LuaSocket_SOURCE_DIR}/src/options.h
		${LuaSocket_SOURCE_DIR}/src/options.c
		${LuaSocket_SOURCE_DIR}/src/timeout.h
		${LuaSocket_SOURCE_DIR}/src/timeout.c
		${LuaSocket_SOURCE_DIR}/src/io.h
		${LuaSocket_SOURCE_DIR}/src/io.c
		${LuaSocket_SOURCE_DIR}/src/usocket.h
		${LuaSocket_SOURCE_DIR}/src/usocket.c
		${LuaSocket_SOURCE_DIR}/src/unix.c
)
SET(luasocket_UNIX_PUBLIC_HEADERS
		${LuaSocket_SOURCE_DIR}/src/unix.h
)

IF(WANTS_BUILD_MODULE_LIBRARY)
	ADD_LIBRARY(luasocket_unix_library_module MODULE ${luasocket_UNIX_PUBLIC_HEADERS} ${luasocket_UNIX})
	SET_TARGET_PROPERTIES(luasocket_unix_library_module PROPERTIES 
		PREFIX ""
		OUTPUT_NAME "core"
	)
	TARGET_LINK_LIBRARIES(luasocket_unix_library_module ${LUA_LIBRARIES} ${LUASOCKET_LINK_FLAGS})
	SET_TARGET_PROPERTIES(luasocket_unix_library_module PROPERTIES COMPILE_FLAGS "${LUASOCKET_C_FLAGS}")
ENDIF(WANTS_BUILD_MODULE_LIBRARY)


IF(WANTS_BUILD_STATIC_LIBRARY)
	ADD_LIBRARY(luasocket_unix_library_static STATIC ${luasocket_UNIX_PUBLIC_HEADERS} ${luasocket_UNIX})
	SET_TARGET_PROPERTIES(luasocket_unix_library_static PROPERTIES OUTPUT_NAME "luasocketunix")
	TARGET_LINK_LIBRARIES(luasocket_unix_library_static ${LUA_LIBRARIES} ${LUASOCKET_LINK_FLAGS})
	SET_TARGET_PROPERTIES(luasocket_unix_library_static PROPERTIES COMPILE_FLAGS "${LUASOCKET_C_FLAGS}")
ENDIF(WANTS_BUILD_STATIC_LIBRARY)

IF(WANTS_BUILD_STATIC_LIBRARY)
	INSTALL(TARGETS
		luasocket_unix_library_static
		#	RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
		#	ARCHIVE DESTINATION lib/static
	)
ENDIF(WANTS_BUILD_STATIC_LIBRARY)

IF(WANTS_BUILD_MODULE_LIBRARY)
	INSTALL(TARGETS luasocket_unix_library_module DESTINATION ${LUAPACKAGE_CDIR}/unix)
ENDIF(WANTS_BUILD_MODULE_LIBRARY)

INSTALL(FILES ${luasocket_UNIX_PUBLIC_HEADERS} DESTINATION include)


