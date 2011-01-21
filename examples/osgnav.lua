#!/home/rpavlik/src/vrjugglua-build/src/vrjlua

print("Loading lua file osgnav.lua")

-- sets the variable "scene"
--loadfile("sceneconfig.lua")
node = osgLua.loadObjectFile('cessna.osg')
scene = osg.PositionAttitudeTransform()
scene:addChild(node)


require("Navigator")
require("osgTools")

osgnav = {position = osg.Vec3d(0, 0, 0)}

function osgnav:initScene()
	print("Setting up position interface")
	self.wand = gadget.PositionInterface("VJWand")

	print("Setting up digital interface")
	self.button = gadget.DigitalInterface("VJButton0")

	print("Creating navigator")
	self.nav = Navigator.create(maxspeed)
	Navigator.useWandTranslation(self.nav, self.wand, self.button)

	print("Setting up scenegraph")
	self.navtransform = osg.PositionAttitudeTransform()
	self.navtransform:addChild(scene)

	print("Attaching to app proxy's scene")
	self.appProxy:getScene():addChild(self.navtransform)
end

function osgnav:preFrame()
	self.position = self.position - self.nav:getTranslation(self.appProxy:getTimeDelta(), self.position)
	self.navtransform:setPosition(self.position)
end


print("App delegate defined, now creating OsgAppProxy")
osgnav.appProxy = vrjApp.OsgAppProxy()

print ("Now setting OsgAppProxy's delegate")
osgnav.appProxy:setAppDelegate(osgnav)

print("Loading config files into kernel")
vrjKernel.loadConfigFile("standalone.jconf")

print("Setting kernel application")
osgnav.appProxy:setActiveApplication()

print("Starting kernel")
vrjKernel.enter()
