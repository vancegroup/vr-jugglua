require("getScriptFilename")

AddAppDirectory = function(required)
	local scriptFn, err = getScriptFilename(3)
	if scriptFn == nil then
		local message = "AddAppDirectory() must be run from a file, not copied-and-pasted, to work properly. We might not be able to find files you're searching for. (Calling AddAppDirectory(true) makes this an error instead of just a warning, stopping your program.) Details: " .. err
		if required then
			error(message, 2)
		else
			print("WARNING: " .. message)
		end
	else
		vrjLua.appendToModelSearchPath(scriptFn)
	end
end
