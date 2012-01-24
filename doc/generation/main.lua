#!/usr/bin/env vrjlua

local orig_globals = require "bootstrap.default_lua_globals"
local luabind_globals = require "luabind_globals"
local is_osg_wrapper = require "is_osg_wrapper"

local vrjlua_luabind = require "vrjlua_luabind"

local do_nothing = function() return "" end

local create_lua_docs = function(name)
	local link = ("See lua documentation for %q"):format(name)
	return function()
		return link
	end
end

local create_luabind_globals_docs = function(name)
	local link = ("See Luabind documentation for %q"):format(name)
	return function()
		return link
	end
end

local create_osgwrapper_docs = function(name)
	local link = ("See OpenSceneGraph documentation for %q"):format(name)
	return function()
		return link
	end
end



local handle_global = function(self, name)
	if orig_globals[name] then
		self[name] = create_lua_docs(name)
	elseif luabind_globals[name] then
		self[name] = create_luabind_globals_docs(name)
	elseif is_osg_wrapper(name) then
		self[name] = create_osgwrapper_docs(name)
	else
		return function() vrjlua_luabind.document_entity(name, _G[name]) end
	end

	return rawget(self, name)
--[[
	-- OK, if we make it this far then we actually have to document.
	print("Will have to actually do documentation for", name)
	return do_nothing]]
end




local global_handlers = setmetatable({}, {__index = handle_global})

global_handlers["osgLua"] = function()
	return "osgLua table. TODO" -- TODO
end

for k, v in pairs(_G) do
	--[[
	if not orig_globals[k] and not luabind_globals[k] then
		print(k, type(v), v)
		if is_osg_wrapper(k) then
			print("Apparently an osg wrapper\n")
		end
		if k == "osgLua"
	end
	]]
	local doc = global_handlers[k]()
	print(k, type(v))
end
