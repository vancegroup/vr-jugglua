return function(name)
	if name == "OpenThreads" then return true end
	if name == "osgLua" then return false end
	return name:match("^osg") ~= nil
end
