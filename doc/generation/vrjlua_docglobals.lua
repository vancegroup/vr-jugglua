#!/usr/bin/env lua

local orig_globals = require "default_lua_globals"
local luabind_globals = require "luabind_globals"
local is_osg_wrapper = require "is_osg_wrapper"

for k, v in pairs(_G) do
	if not orig_globals[k] and not luabind_globals[k] then
		print(k, type(v), v)
		if is_osg_wrapper(k) then
			print("Apparently an osg wrapper\n")
		end
	end
end
