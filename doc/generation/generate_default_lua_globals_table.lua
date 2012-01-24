#!/usr/bin/env lua
print "return {"
for k, v in pairs(_G) do
	print( ([[ [%q] = true, ]]):format(k))
end
print ")"
