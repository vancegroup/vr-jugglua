
add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/squishbootstrap"
	COMMAND ${LUA_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/squish.lua" -q --with-minify --with-uglify --with-compile --with-virtual-io --output="${CMAKE_CURRENT_BINARY_DIR}/squishbootstrap" "${CMAKE_CURRENT_SOURCE_DIR}"
	DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/squish.lua")

add_custom_command(OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/gunzip.lua"
	COMMAND ${LUA_EXECUTABLE} "${CMAKE_CURRENT_BINARY_DIR}/squishbootstrap" -q "${CMAKE_CURRENT_SOURCE_DIR}/gzip" --output="${CMAKE_CURRENT_SOURCE_DIR}/gunzip.lua"
	DEPENDS
	"${CMAKE_CURRENT_BINARY_DIR}/squishbootstrap"
	"${CMAKE_CURRENT_SOURCE_DIR}/gzip/squish.gzip.lua")
	
add_custom_command(OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/squish.debug"
	COMMAND ${LUA_EXECUTABLE} "${CMAKE_CURRENT_BINARY_DIR}/squishbootstrap" -q "${CMAKE_CURRENT_SOURCE_DIR}/debug" --output="${CMAKE_CURRENT_SOURCE_DIR}/squish.debug"
	DEPENDS
	"${CMAKE_CURRENT_BINARY_DIR}/squishbootstrap"
	"${CMAKE_CURRENT_SOURCE_DIR}/debug/squish.debug.lua")

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/squish"
	COMMAND ${LUA_EXECUTABLE} "${CMAKE_CURRENT_BINARY_DIR}/squishbootstrap" -q --with-minify --with-uglify --with-compile --with-virtual-io  --with-gzip --with-debug --output="${CMAKE_CURRENT_BINARY_DIR}/squish" "${CMAKE_CURRENT_SOURCE_DIR}"
	DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/squishbootstrap" "${CMAKE_CURRENT_SOURCE_DIR}/squish.debug" "${CMAKE_CURRENT_SOURCE_DIR}/gunzip.lua")

add_custom_target(squish ALL SOURCES squish.lua DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/squish")

set(SQUISH_COMMAND ${LUA_EXECUTABLE} "${CMAKE_CURRENT_BINARY_DIR}/squish" -q --use-http --minify-level=debug --debug PARENT_SCOPE)

add_dependencies(lua_modules squish)
