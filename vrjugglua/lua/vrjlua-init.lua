
-- Enable help by default
require("help")

-- Override print function with threadsafe Juggler one
do
	local oldprint = print
	print = function(s)
		vrjLua.safePrint(tostring(s))
	end
end