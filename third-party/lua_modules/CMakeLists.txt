if(BUILD_EMBEDDED_LUA)
	set(LUA_EXECUTABLE lua-interp)
endif()

add_custom_target(lua_modules ALL)
add_subdirectory(squish)
add_subdirectory(lua-docstring)
set(LUA_MODULES lua-docstring)

if(BUILD_VERSE)
	add_subdirectory(verse)
	list(APPEND LUA_MODULES verse)
endif()
