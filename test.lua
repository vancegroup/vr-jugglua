local docstrings = setmetatable({}, {__mode = "kv"})
local function createConcat(decorationFunc)
	local mt = {__concat = decorationFunc}
	return function(...)
		return setmetatable({...}, mt)
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

function docstring(docs)
	local mt = {}
	function mt.__concat(a, f)
		docstrings[f] = {docs}
		return f
	end
	return setmetatable({}, mt)
end


function help(f)
	if docstrings[f] then
		print("Help:", table.concat(docstrings[f], ","))
	else
		print("No help available!")
	end
end

a = docstring[[This is an example]] .. function()
	print("this is the function")
end

print("Before calling a")
print(a())

print("before calling help a")
help(a)

print("before defining random")
random =
  docstring[[Compute random number.]] ..
  decorator("number", '->', "number") ..
  function(n)
    return math.random(n)
  end

print("before printing random")
print(random)

print("before printing results of random call")
print(random(5))