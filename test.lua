local docstrings = setmetatable({}, {__mode = "kv"})
local function createConcat(decorationFunc)
	local mt = {__concat = decorationFunc}
	return function(...)
		return setmetatable({...}, mt)
	end
end

docstring = createConcat(function(a,f)
	    return function(...)
	      print("decorator", table.concat(a, ","), ...)
	      return f(...)
	    end
	  end
)

typecheck = docstring

a = docstring[[This is an example]] .. function()
	print("this is the function")
end

a()

random =
  docstring[[Compute random number.]] ..
  typecheck("number", '->', "number") ..
  function(n)
    return math.random(n)
  end

print(random)
print(random(5))