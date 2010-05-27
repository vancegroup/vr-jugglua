print("Loading lua file")

testapp = {}

function testapp:initScene()
	print("In initScene")
end

function testapp:preFrame()
	print("In preFrame")
end


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

print("Stopping kernel")
vrjKernel.stop()
