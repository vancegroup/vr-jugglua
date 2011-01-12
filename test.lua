local mt = {__concat =
  function(a,f)
    return function(...)
      print("decorator", table.concat(a, ","), ...)
      return f(...)
    end
  end
}

function typecheck(...)
  return setmetatable({...}, mt)
end

function docstring(...)
  return setmetatable({...}, mt)
end

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