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

print("CTEST_FULL_OUTPUT")

-- generated with: 
-- ls osgwrapper_*|sed s/osgwrapper_/\"/ | sed s/.so/\"\;/
wrappers = {
"OpenThreads";
"osgDB";
"osgFX";
"osgGA";
"osgManipulator";
"osgParticle";
"osgShadow";
"osgSim";
"osg";
"osgTerrain";
"osgText";
"osgUtil";
"osgViewer";
"osgVolume";
"osgWidget";
}

for _, wrapper in ipairs(wrappers) do
	print("Loading wrapper for " .. wrapper)
	osgLua.loadWrapper(wrapper)
end
print("Wrapper loading done, will dump info.")

types = osgLua.getTypes()
for t,d in ipairs( types ) do
	showInfo(d)
end

