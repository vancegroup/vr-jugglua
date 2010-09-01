require("libosgLua")
function showInfo(instance) 
	local c = osgLua.getTypeInfo(instance)
	if c  then 
		print("name = ",c.name)
		for i,v in ipairs(c.constructors) do
	  	 print(string.format("\tconstructor(%2d) %s",i,v))
		end
		for i,v in ipairs(c.methods) do
	  	 print(string.format("\tmethod(%2d) %s",i,v))
		end
	else
		print(instance.." ** not found **")
	end
end

osgLua.loadWrapper("osg")
osgLua.loadWrapper("osgText")
osgLua.loadWrapper("osgGA")
osgLua.loadWrapper("osgProducer")

types = osgLua.getTypes()
for t,d in ipairs( types ) do
	showInfo(d)
end

