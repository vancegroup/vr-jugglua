do
	class 'lua_testapp' (vrj.OsgApp)

	function lua_testapp:__init()
		print("App object created")
		self.counter = 0
		vrj.OsgApp:__init()
	end

	function lua_testapp:initScene()
		print("in lua_testapp:initScene()")
	end

	function lua_testapp:preFrame()
		self.counter = self.counter + 1
		if self.counter > 10 then vrjKernel.stop() end
		print ("preFrame number" .. tostring(self.counter))
	end

	app = lua_testapp()
	print("After instantiating the app")
	print(type(app))
	vrjKernel.setApplication(vrj.OsgApp:getAppPointer())

	--osgLua.loadWrapper("osg")
	--osgLua.loadWrapper("osgDB")
	return
end
