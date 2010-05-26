# 2009-2010 Ryan Pavlik <rpavlik@iastate.edu>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

add_subdirectory(lua-5.1.4/src)
set(LUA_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/lua-5.1.4/src")
set(LUA_INCLUDE_DIRS ${LUA_INCLUDE_DIRS} PARENT_SCOPE)
set(LUA_LIBRARIES lua)
set(LUA_LIBRARIES ${LUA_LIBRARIES} PARENT_SCOPE)

add_subdirectory(luabind)
set(LUABIND_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/luabind" ${LUA_INCLUDE_DIRS})
set(LUABIND_INCLUDE_DIRS ${LUABIND_INCLUDE_DIRS} PARENT_SCOPE)

add_subdirectory(osgLua)
set(OSGLUA_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/osgLua/include" "${CMAKE_CURRENT_SOURCE_DIR}/osgLua/src" ${LUA_INCLUDE_DIRS})
set(OSGLUA_INCLUDE_DIRS ${OSGLUA_INCLUDE_DIRS} PARENT_SCOPE)
