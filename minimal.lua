package.cpath = "/home/rpavlik/deps/osgLua/lib/?.so;" .. package.cpath

require("libosgLua")
print("Loading lua file")

function initScene()
	print("in initScene()")
	return true
end

--osgLua.loadWrapper("osg")
--osgLua.loadWrapper("osgDB")
