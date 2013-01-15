return function(name)
	if name:match("^OpenThreads") then return true end
	if name:match("^osgLua") then return false end
	return name:match("^osg") ~= nil
end
