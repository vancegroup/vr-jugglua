#!/usr/bin/env vrjlua

local handled = {}

local osgLuaDocs = setmetatable({
	["getTypes"] = {
		['description'] = [[Returns a table of all osgLua types, listed as their fully-qualified C++ name.]],
		['params'] = {}
	},
	["saveObjectFile"] = {
		['description'] = [[Pass an OSG object (node, etc) and a filename (likely ending in .osg).
	Saves the given object to the named file]],
		['params'] = {"object", "filename"}
	},
	["createByName"] = {
		['description'] = [[Pass a string naming a type, and optionally parameters. Returns a new instance of the named type passing parameters to the constructor.
	Usually can be ignored in favor of using the global tables directly to access constructors.]],
		['params'] = {"typename", "..."}
	},
	["loadObjectFile"] = {
		['description'] = [[Pass a filename. Returns OSG object (node, etc) loaded from the named file.]],
		['params'] = {"filename"}
	},
	["NodeCallback"] = {
		['description'] = [[Pass a function. Returns an osg.NodeCallback that calls the given function.]],
		['params'] = {"callback"}
	},
	["getTypeInfo"] = {
		['description'] = [[Pass a value or string. Returns a table with information about the given value's
	type or the type specified by the given string.]],
		['params'] = {"valOrTypename"}
	},
	["loadWrapper"] = {
		['description'] = [[Pass a string indicating an OSG nodekit to load (e.g. "osg", "osgText").
	Loads the wrapper for that functionality, creating a corresponding global table providing access to it.]],
		['params'] = {"nodekit"}
	},
},
{
	__index = function(_, tname)
		return {
			['description'] = ([[Explicitly creates and returns an osgLua value of type %q]]):format(tname),
			['params'] = { "number" }
		}
	end
})

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

local outputCommentLine = function(s)
	output("-- " .. s:gsub('\n', '\n-- '))
end

local outputMethod = function(methodName)
	if handled[methodName] then return end
	handled[methodName] = true
	local currDoc = osgLuaDocs[methodName]
	output "---"
	outputCommentLine(currDoc.description)
	output(("function %s(%s) end"):format(methodName, table.concat(currDoc.params, ", ")))
	output()
end

output "---"
outputCommentLine [[
This module uses the osgIntrospection library to provide nearly complete access to
the C++ API of OpenSceneGraph.  In general, C++ OSG code can be converted with a few minor changes:

  * Namespace navigation and calls to static members are done using `.`, e.g. `osg.Group`
  * Member functions are called with `:`, e.g., `group:addChild(node)`
  * Some direct property access is available.
]]
output "module 'osgLua'"
output()



output "--- Explicit number type constructors"
outputCommentLine [[@section numberconstructors]]

for methodName, _ in pairs(osgLua) do
	if methodName:find("^GL") then
		outputMethod(methodName)
	end
end


output "--- Introspection functions"
outputCommentLine [[@section introspect]]

for _, methodName in ipairs{"getTypeInfo", "getTypes"} do
	outputMethod(methodName)
end

output "--- Other"
outputCommentLine [[@section other]]

for methodName, _ in pairs(osgLua) do
	outputMethod(methodName)
end

print "*** luadoc generation for osgLua completed successfully! ***"



