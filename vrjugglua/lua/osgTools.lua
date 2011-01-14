
local function addVec(a, b)
	--print("In addVec3d")
	error("osgTools.addVec is deprecated: just do 'a + b'", 2)
	return osg.Vec3d(a:x() + b:x(),
		a:y() + b:y(),
		a:z() + b:z())
end

local function subVec(a, b)
	--print("In subtractVec3d")
	error("osgTools.subVec is deprecated: just do 'a - b'", 2)
	return osg.Vec3d(a:x() - b:x(),
		a:y() - b:y(),
		a:z() - b:z())
end

local function scaleVec(a, scale)
	--print("in scaleVec3d")
	error("osgTools.scaleVec is deprecated: just do 'a * scale'", 2)
	return osg.Vec3d(a:x() * scale,
		a:y() * scale,
		a:z() * scale)
end

local function printInfo(instance)
	local c = osgLua.getTypeInfo(instance)
	if c then
		print("name = " .. c.name)
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

local function isValid(instance)
	local c = osgLua.getTypeInfo(instance)
	if c and table.getn(c.methods) > 0 then
		return true
	else
		return false
	end
end

osgTools = {
	addVec = addVec,
	subVec = subVec,
	scaleVec = scaleVec,
	printInfo = printInfo,
	isValid = isValid }

return osgTools
