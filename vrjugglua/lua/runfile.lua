if runfile == nil then
	local isValidPath = function(fn)
		return (fn ~= nil) and (fn ~= "")
	end
	runfile = function(fn)
		local fullPath
		for _, try in ipairs{fn, fn .. ".lua"} do
			if isValidPath(try) then
				fullPath = try
				break
			end
		end

		if isValidPath(fullPath) then
			print(("Running %s - found at %s"):format(fn, fullPath))
			return dofile(fullPath)
		else
			error(("Could not find %s in model search path to run!"):format(fn), 2)
		end
	end
end
