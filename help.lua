local mt = {}
help = setmetatable({}, mt)
local docstrings = setmetatable({}, {__mode = "kv"})
local helpExtensions = {}

function mt:__call(obj,...)
	local helpContent = help.lookup(obj)
	
	if helpContent then
		print("Help:\t" .. help.formatHelp(helpContent))
	else
		print("No help available!")	
	end
end

function help.formatHelp(h)
  if type(h) == "string" then
    return h
  elseif type(h) == "table" then
    return table.concat(h, ",")
  else
    return h
  end
end

function help.lookup(obj)
	if docstrings[obj] then
		 return docstrings[obj]
	end
	for _, v in ipairs(helpExtensions) do
		local helpContent = v(obj)
		if helpContent then 
			return helpContent
		end
	end
	return nil
end

function help.docstring(docs)
	local mt = {}
	-- handle the .. operator for inline documentation
	function mt.__concat(a, f)
		docstrings[f] = docs
		return f
	end
	
	-- hanadle the () operator for after-the-fact docs
	function mt:__call(f)
		docstrings[f] = docs
		return f
	end
	return setmetatable({}, mt)
end

function help.addHelpExtension(func)
	table.insert(helpExtensions, func)
end

if class_info then
  --require("helpLuabind")
end

if osgLua then
  --require("helpOsgLua")
end
