#!/bin/sh

PROJECT=$(cd $(dirname $0) && pwd)
includes="Callback Config.in LoadWrapper LuaInclude Script Value"
astyle -n --recursive "${PROJECT}/src/osgLua/*.cpp" --recursive "${PROJECT}/src/osgLua/*.h"
for fn in ${includes}; do
	astyle -n "${PROJECT}/include/osgLua/${fn}"
done

