
-- Enable help by default
require("help")

-- Override print function with threadsafe Juggler one
do
	local oldprint = print
	print = function(s)
		vrjLua.safePrint(tostring(s))
	end
end

-- Add support for loading osg wrappers using require()
local function osgLuaSearcherFunction(modname)
	-- first see if it starts with osg - assume that's needed
	if modname:find("osg") then
		-- OK, now see if osgLua errors when trying to load that wrapper
		if pcall(function() osgLua.loadWrapper(modname) end) then
			-- no error: this is successful! return a no-op loader funciton
			return function(modname) --[[ no-op function: already loaded ]] end
		else
			-- error: couldn't load a wrapper, tell require() why
			return "\n\tosgLua: no valid osg wrapper for '" .. modname .. "' even though the name starts with osg"
		end
	end

	return "\n\tosgLua: '" .. modname .. "' does not start with osg so assumed not an osg wrapper"
end

table.insert(package.loaders, osgLuaSearcherFunction)
