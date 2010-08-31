# 2009-2010 Ryan Pavlik <rpavlik@iastate.edu>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC


if(BUILD_EMBEDDED_LUA)
	set(LUA_EXECUTABLE lua-interp)
endif()

add_subdirectory(squish)

if(BUILD_VERSE)
	add_subdirectory(verse)
endif()