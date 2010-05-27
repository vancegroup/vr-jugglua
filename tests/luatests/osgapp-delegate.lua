print("Loading lua file")

testapp = {}

function testapp:initScene()
	print("In initScene")
end

function testapp:preFrame()
	print("In preFrame")
end


print("App defined, now setting delegate")
vrjApp.setAppDelegate(testapp)

print("Loading config files into kernel")
vrjKernel.loadConfigFile("standalone.jconf")
