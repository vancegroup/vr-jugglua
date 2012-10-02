if runfile == nil then
	local isValidPath = function(fn, soughtFn)
		return (fn ~= nil) and (fn ~= "") and (fn ~= soughtFn)
	end
	runfile = function(fn)
		local fullPath
		for _, try in ipairs{fn, fn .. ".lua"} do
			local testPath = vrjLua.findInModelSearchPath(try)
			if isValidPath(testPath, try) then
				fullPath = testPath
				break
			end
		end

		if fullPath ~= nil then
			print(("Running %s - found at %s"):format(fn, fullPath))
			return dofile(fullPath)
		else
			error(("Could not find %s in model search path to run!"):format(fn), 2)
		end
	end
end
