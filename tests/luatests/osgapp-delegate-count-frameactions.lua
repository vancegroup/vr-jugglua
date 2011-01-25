print("Loading lua file")
print("CTEST_FULL_OUTPUT")


require("Actions")

testapp = {
	max = 10
}


function testapp:initScene()
	print("In initScene")
	Actions.addFrameAction(function()
		for i=1,testapp.max do
			print("i = " .. tostring(i))
			Actions.waitForRedraw()
		end
		print("Counter at maximum")
		vrjKernel.stop()
		os.exit(0)
	end)
end



function testapp:latePreFrame(dt)
	Actions.updateFrameActions(dt)
end



print("App delegate defined, now creating OsgAppProxy")
testapp.appProxy = vrjApp.OsgAppProxy()
Actions.setAppProxy(testapp.appProxy)

print ("Now setting OsgAppProxy's delegate")
testapp.appProxy:setAppDelegate(testapp)

print("Loading config files into kernel")
vrjKernel.loadConfigFile("standalone.jconf")

print("Setting kernel application")
testapp.appProxy:setActiveApplication()

print("Starting kernel")
vrjKernel.enter()
