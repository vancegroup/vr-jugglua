-- Override print function with threadsafe Juggler one
do
	local oldprint = print
	print = function(s)
		vrjKernel.safePrint(tostring(s))
	end
end