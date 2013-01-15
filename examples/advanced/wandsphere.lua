require("Scene")

wandXform = osg.MatrixTransform()
wandXform:addChild(Sphere{radius = 0.0125, position = {0, 0, -0.25}})

app = {}
function app:initScene()
	print("Setting up position interface")
	self.head = gadget.PositionInterface("VJHead")

	self.wand = gadget.PositionInterface("VJWand")

	print("Attaching to app proxy's scene")
	self.appProxy:getScene():addChild(wandXform)
end

function app:preFrame()
	wandXform:setMatrix(self.wand.matrix)
	print(self.head.position)
end


print("App delegate defined, now creating OsgAppProxy")
app.appProxy = vrjApp.OsgAppProxy()

print ("Now setting OsgAppProxy's delegate")
app.appProxy:setAppDelegate(app)

print("Loading config files into kernel")
vrjKernel.loadConfigFile("V:/Applications/Vances_group/jconf/IS900TwoWall.jconf")
vrjKernel.loadConfigFile("V:/Applications/Vances_group/jconf/simview.mixin.jconf")

print("Setting kernel application")
app.appProxy:setActiveApplication()

print("Starting kernel")
vrjKernel.enter()