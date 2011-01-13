local docstrings = setmetatable({}, {__mode = "kv"})
local function createConcat(decorationFunc)
	local mt = {__concat = decorationFunc}
	return function(...)
		return setmetatable({...}, mt)
	end
end

local mt = {}
local help = setmetatable({}, mt)

function mt:__call(obj,...)
	print(arg.n)
	if docstrings[obj] then
		print("Help:", table.concat(docstrings[obj], ","))
	else
		print("No help available!")
	end
end

decorator = createConcat(function(a,f)
	    return function(...)
	      print("decorator", table.concat(a, ","), ...)
	      return f(...)
	    end
	  end
)
--[[
docstring = createConcat(
	function(a,f)
		return function(...)
			if docstrings[f] then
				table.insert(docstrings[f], a)
			else
				docstrings[f] = {a}
			end
			return f
		end
	end
)]]

function help.docstring(docs, ...)
	print("arg to docstring: " .. docs)
	print("Extra args to docstring: " .. arg.n)
	local mt = {}
	function mt.__concat(a, f)
		docstrings[f] = {docs, unpack(arg)}
		return f
	end
	function mt:__call(f)
		docstrings[f] = {docs, unpack(arg)}
		return f
	end
	return setmetatable({}, mt)
end


a = help.docstring[[This is an example]] .. function()
	print("this is the function")
end

print("Before calling a")
print(a())

print("before calling help a")
help(a)

print("before defining b")

--docstring[[a second documented func]] ..
function b()
	print("this is the second function")
end

help.docstring([[a second documented func]])(b)

help(b)

print("before defining random")
random =
  help.docstring[[Compute random number.]] ..
  decorator("number", '->', "number") ..
  function(n)
    return math.random(n)
  end

print("before printing random")
print(random)

print("before printing results of random call")
print(random(5))