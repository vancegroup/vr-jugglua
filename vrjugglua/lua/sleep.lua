-- see http://lua-users.org/wiki/SleepFunction
function sleep(n) -- seconds
	if os.getenv("USER") ~= nil then
		-- Unix
		os.execute("sleep " .. tonumber(n))
	elseif os.getenv("PROGRAMFILES") ~= nil then
		-- Windows
		os.execute("ping -n " .. tonumber(n) .. " localhost > NUL")
	else
		-- No idea
		local t0 = os.clock()
		print("Sleeping for " .. tostring(n) .. " seconds")
		while os.difftime(os.clock(), t0) <= n and os.clock() > t0 do end
	end
end
