function showInfo(instance) 
	local c = osgLua.getTypeInfo(instance)
	if c  then 
		print("name = ",c.name)
		if c.constructors then
			for i,v in ipairs(c.constructors) do
		  	 print(string.format("\tconstructor(%2d) %s",i,v))
			end
		end
		if c.methods then
			for i,v in ipairs(c.methods) do
		  	 print(string.format("\tmethod(%2d) %s",i,v))
			end
		end
	else
		print(instance.." ** not found **")
	end
end

print("CTEST_FULL_OUTPUT")
osgLua.loadWrapper("OpenThreads")
types = osgLua.getTypes()
for t,d in ipairs( types ) do
	print("TYPE: ", t)
	showInfo(d)
	--[[
	result = pcall(function() showInfo(d) end)
	if not result then
		print("errored out! \n\n")
	end
	]]
end

