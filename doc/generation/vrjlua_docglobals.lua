#!/usr/bin/env lua

local orig_globals = require "default_lua_globals"

for k, v in pairs(_G) do
	if not orig_globals[k] then
		print(k)
	end
end
