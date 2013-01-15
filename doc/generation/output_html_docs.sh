#!/bin/sh
cd $(dirname $0)
#export LUA_PATH=$(pwd)/LDoc/?.lua:${LUA_PATH}
lua -lluarocks.loader LDoc/ldoc.lua .


