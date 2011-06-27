--[[
When you pass a path to load a model, it will search in what's known as the "model search path." Basically, anything that you put in the filename to load will be pasted after each path in the model search path and it will check there for the file.]]

-- This method returns a Lua table containing all of the paths in the model search path.
paths = vrjLua.getModelSearchPath()

-- We can print them out like this:
for num, path in ipairs(paths) do
	print(tostring(num) .. " - " .. path)
end


