
local function addVec(a, b)
	--print("In addVec3d")
	return osg.Vec3d(a:x() + b:x(),
		a:y() + b:y(),
		a:z() + b:z())
end

local function subVec(a, b)
	--print("In subtractVec3d")
	return osg.Vec3d(a:x() - b:x(),
		a:y() - b:y(),
		a:z() - b:z())
end

local function scaleVec(a, scale)
	--print("in scaleVec3d")
	return osg.Vec3d(a:x() * scale,
		a:y() * scale,
		a:z() * scale)
end

local function printInfo(instance)
	local c = osgLua.getTypeInfo(instance)
	if c then
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

osgTools = {
	addVec = addVec,
	subVec = subVec,
	scaleVec = scaleVec,
	printInfo = printInfo }

return osgTools
