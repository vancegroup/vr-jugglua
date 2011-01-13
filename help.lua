local docstrings = setmetatable({}, {__mode = "kv"})
local mt = {}
local helpExtensions = {}
help = setmetatable({}, mt)

function mt:__call(obj,...)
	print(arg.n)
	local helpContent = help.lookup(obj)
	
	if helpContent then
		print("Help:", helpContent)
	else
		print("No help available!")	
	end
end

function help.lookup(obj)
	if docstrings[obj] then
		 return table.concat(docstrings[obj], ",")
	end
	for _, v in ipairs(helpExtensions) do
		local helpContent = v(obj)
		if helpContent then 
			return helpContent
		end
	end
end

function help.docstring(docs, ...)
	print("arg to docstring: " .. docs)
	print("Extra args to docstring: " .. arg.n)
	local mt = {}
	-- handle the .. operator for inline documentation
	function mt.__concat(a, f)
		docstrings[f] = {docs, unpack(arg)}
		return f
	end
	
	-- hanadle the () operator for after-the-fact docs
	function mt:__call(f)
		docstrings[f] = {docs, unpack(arg)}
		return f
	end
	return setmetatable({}, mt)
end

function help.addHelpExtension(func)
	table.insert(helpExtensions, func)
end
