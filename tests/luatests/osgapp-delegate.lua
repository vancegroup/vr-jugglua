print("Loading lua file")
print("CTEST_FULL_OUTPUT")

testapp = {

	initScene = function (self)
		print("In initScene")
	end,

	preFrame = function (self)
		print("In preFrame - Stopping kernel")
		vrjKernel.stop()
	end
}


print("App delegate defined, now creating OsgAppProxy")
app = vrjApp.OsgAppProxy()

print ("Now setting OsgAppProxy's delegate")
app:setAppDelegate(testapp)

print("Loading config files into kernel")
vrjKernel.loadConfigFile("standalone.jconf")

print("Setting kernel application")
app:setActiveApplication()

print("Starting kernel")
vrjKernel.start()

print("Waiting for kernel stop")
vrjKernel.waitForKernelStop()
