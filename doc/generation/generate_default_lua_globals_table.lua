#!/usr/bin/env lua
local writer = print
if arg[1] then
	local f = assert(io.open(f, 'w'))
	writer = function(s)
		f:write(s, '\n')
	end
end

writer "return {"
for k, v in pairs(_G) do
	writer( ([[ [%q] = true, ]]):format(k))
end
writer ")"
