# Locate Lua library
# This module defines
# LUA_LIBRARY
# LUA_FOUND, if false, do not try to link to lua
# LUA_INCLUDE_DIR, where to find the headers
#
# $LUA_DIR is an environment variable to find Lua
#
# Created by Jose L. Hidalgo. 

FIND_PATH(LUA_INCLUDE_DIR lua.h
    $ENV{LUA_DIR}/include
    $ENV{LUA_DIR}
    $ENV{LUA_ROOT}
    $ENV{LUA_ROOT}/include
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local/include
    /usr/include
    /sw/include # Fink
    /opt/local/include # DarwinPorts
    /opt/csw/include # Blastwave
    /opt/include
    [HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session\ Manager\\Environment;LUA_ROOT]/include
    /usr/freeware/include
)

FIND_LIBRARY(LUA_LIBRARY
	NAMES lua
	PATHS
	$ENV{OSG_DIR}/lib
	$ENV{OSG_DIR}
	$ENV{OSGDIR}/lib
	$ENV{OSGDIR}
	$ENV{OSG_ROOT}/lib
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local/lib
	/usr/lib
	/sw/lib
	/opt/local/lib
	/opt/csw/lib
	/opt/lib
	[HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session\ Manager\\Environment;LUA_ROOT]/lib
	/usr/freeware/lib64
)

SET(LUA_FOUND "NO")
IF(LUA_LIBRARY AND LUA_INCLUDE_DIR)
    SET(LUA_FOUND "YES")
ENDIF(LUA_LIBRARY AND LUA_INCLUDE_DIR)
