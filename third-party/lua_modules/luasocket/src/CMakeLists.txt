
IF(WIN32)
	SET(luasocket_PLATFORM_SPECIFIC
		${LuaSocket_SOURCE_DIR}/src/wsocket.h
		${LuaSocket_SOURCE_DIR}/src/wsocket.c
	)
ELSEIF(UNIX)
	SET(luasocket_PLATFORM_SPECIFIC
		${LuaSocket_SOURCE_DIR}/src/usocket.h
		${LuaSocket_SOURCE_DIR}/src/usocket.c
	)
ENDIF(WIN32)

SET(luasocket_SOCKET
		${LuaSocket_SOURCE_DIR}/src/luasocket.c
		${LuaSocket_SOURCE_DIR}/src/inet.h
		${LuaSocket_SOURCE_DIR}/src/inet.c
		${LuaSocket_SOURCE_DIR}/src/tcp.h
		${LuaSocket_SOURCE_DIR}/src/tcp.c
		${LuaSocket_SOURCE_DIR}/src/udp.h
		${LuaSocket_SOURCE_DIR}/src/udp.c
		${LuaSocket_SOURCE_DIR}/src/except.h
		${LuaSocket_SOURCE_DIR}/src/except.c
		${LuaSocket_SOURCE_DIR}/src/select.h
		${LuaSocket_SOURCE_DIR}/src/select.c
		${LuaSocket_SOURCE_DIR}/src/timeout.h
		${LuaSocket_SOURCE_DIR}/src/timeout.c
		${LuaSocket_SOURCE_DIR}/src/buffer.h
		${LuaSocket_SOURCE_DIR}/src/buffer.c
		${LuaSocket_SOURCE_DIR}/src/io.h
		${LuaSocket_SOURCE_DIR}/src/io.c
		${LuaSocket_SOURCE_DIR}/src/auxiliar.h
		${LuaSocket_SOURCE_DIR}/src/auxiliar.c
		${LuaSocket_SOURCE_DIR}/src/options.h
		${LuaSocket_SOURCE_DIR}/src/options.c
		${luasocket_PLATFORM_SPECIFIC}
)

SET(luasocket_PUBLIC_HEADERS
		${LuaSocket_SOURCE_DIR}/src/luasocket.h
)


SET(luasocket_RESOURCE_FILES
		${LuaSocket_SOURCE_DIR}/src/http.lua
		${LuaSocket_SOURCE_DIR}/src/tp.lua
		${LuaSocket_SOURCE_DIR}/src/ftp.lua
		${LuaSocket_SOURCE_DIR}/src/smtp.lua
		${LuaSocket_SOURCE_DIR}/src/url.lua
)

SET(luasocketcommon_RESOURCE_FILES
		${LuaSocket_SOURCE_DIR}/src/ltn12.lua
		${LuaSocket_SOURCE_DIR}/src/socket.lua
		${LuaSocket_SOURCE_DIR}/src/mime.lua
)

IF(WANTS_BUILD_MODULE_LIBRARY)
	ADD_LIBRARY(luasocket_library_module MODULE ${luasocket_PUBLIC_HEADERS} ${luasocket_SOCKET})
	SET_TARGET_PROPERTIES(luasocket_library_module PROPERTIES 
		PREFIX ""
		OUTPUT_NAME "core"
	)
	TARGET_LINK_LIBRARIES(luasocket_library_module ${LUA_LIBRARIES} ${LUASOCKET_LINK_FLAGS})
	SET_TARGET_PROPERTIES(luasocket_library_module PROPERTIES COMPILE_FLAGS "${LUASOCKET_C_FLAGS}")

ENDIF(WANTS_BUILD_MODULE_LIBRARY)

IF(WANTS_BUILD_STATIC_LIBRARY)
	ADD_LIBRARY(luasocket_library_static STATIC ${luasocket_PUBLIC_HEADERS} ${luasocket_SOCKET})
	SET_TARGET_PROPERTIES(luasocket_library_static PROPERTIES OUTPUT_NAME "luasocket")
	SET_TARGET_PROPERTIES(luasocket_library_static PROPERTIES COMPILE_FLAGS "${LUASOCKET_C_FLAGS}")
	TARGET_LINK_LIBRARIES(luasocket_library_static ${LUA_LIBRARIES} ${LUASOCKET_LINK_FLAGS})


ENDIF(WANTS_BUILD_STATIC_LIBRARY)


IF(WANTS_BUILD_STATIC_LIBRARY)
	INSTALL(TARGETS
		luasocket_library_static
		#	RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
		#	ARCHIVE DESTINATION lib/static
	)

ENDIF(WANTS_BUILD_STATIC_LIBRARY)

IF(WANTS_BUILD_MODULE_LIBRARY)
	INSTALL(TARGETS luasocket_library_module DESTINATION ${LUAPACKAGE_CDIR}/socket)
ENDIF(WANTS_BUILD_MODULE_LIBRARY)

INSTALL(FILES ${luasocket_PUBLIC_HEADERS} DESTINATION include)

INSTALL(FILES ${luasocket_RESOURCE_FILES} DESTINATION ${LUAPACKAGE_LDIR}/socket)
INSTALL(FILES ${luasocketcommon_RESOURCE_FILES} DESTINATION ${LUAPACKAGE_LDIR})

ADD_SUBDIRECTORY(mime)
IF(UNIX)
	ADD_SUBDIRECTORY(unix)
ENDIF(UNIX)


