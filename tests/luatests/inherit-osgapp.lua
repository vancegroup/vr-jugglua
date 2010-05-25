--class 'lua_testapp' (vrj.OsgApp)

--function lua_testapp:initScene()
--	print("in lua_testapp:initScene()")
--end

vrj.stopKernel()

print("Before initScene()")
function initScene()
	print("in initScene()")
	return true
end
print("After defining initScene()")

--app = lua_testapp()
--vrj.setApplication(app)

--osgLua.loadWrapper("osg")
--osgLua.loadWrapper("osgDB")
