local m = {}

local handleNumberConstructor = function(tname)
	return {
		['description'] = ([[Explicitly creates and returns an osgLua value of type %q]]):format(tname),
		['params'] = { "number" }
	}
end

local osgLuaDocs = setmetatable({
	["getTypes"] = {
		['description'] = [[ ]], 
		['params'] = {}
	},
	["saveObjectFile"] = {
		['description'] = [[ ]], 
		['params'] = {"object", "filename"}
	},
	["createByName"] = {
		['description'] = [[ ]], 
		['params'] = {}
	},
	["loadObjectFile"] = {
		['description'] = [[ ]], 
		['params'] = {"filename"}
	},
	["NodeCallback"] = {
		['description'] = [[ ]], 
		['params'] = {}
	},
	["getTypeInfo"] = {
		['description'] = [[ ]], 
		['params'] = {}
	},
	["loadWrapper"] = {
		['description'] = [[ ]], 
		['params'] = {}
	},
}, {__index = function(_, tname) return handleNumberConstructor(tname) end })

local warn = require "docgen_utils.warning"

local f = assert(io.open('osgLua.luadoc', 'w'))


local outputToFile = function(s)
	if s ~= nil then
		f:write(s, '\n')
	else
		f:write('\n')
	end
end

local output = outputToFile --print



output "module 'osgLua'"
output()

local outputCommentLine = function(s)
	output("-- " .. s:gsub('\n', '\n-- '))
end

for k, _ in pairs(osgLua) do

	local currDoc = osgLuaDocs[k]
	output "---"
	outputCommentLine(currDoc.description)
	output(("function %s(%s) end"):format(k, table.concat(currDoc.params, ", ")))
	output()
--[===[
	if k:find("^GL") then
		m[k] = handleNumberConstructor(k)
	else
		print(([==[	[%q] = [[ ]], ]==]):format(k))
--		warn("osgLua." .. k, "No documentation defined!")
	end]===]
	
end


