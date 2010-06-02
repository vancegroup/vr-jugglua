#!/home/rpavlik/src/vrjugglua-build/src/vrjlua

require("Navigator")
require("osgTools")

osgnav = {position = osg.Vec3d(0, 0, 0)}

function osgnav:initScene()
	print("Setting up position interface")
	local wand = gadget.PositionInterface("VJWand")

	print("Setting up digital interface")
	local button = gadget.DigitalInterface("VJButton0")

	print("Creating navigator")
	self.nav = Navigator.create(maxspeed)
	Navigator.useWandTranslation(self.nav, wand, button)

	print("Setting up scenegraph")
	navtransform = osg.PositionAttitudeTransform()

	print("Attaching to app proxy's scene")
	self.appProxy:getScene():addChild(navtransform)
end

function osgnav:preFrame()
	self.position = osgTools.subVec(self.position, self.nav:getTranslation(self.appProxy:getTimeDelta(), self.position))
	navtransform:setPosition(self.position)
end

function osgnav:postFrame()
	runbuf:runBuffer()
end

print("App delegate defined, now creating OsgAppProxy")
osgnav.appProxy = vrjApp.OsgAppProxy()

print ("Now setting OsgAppProxy's delegate")
osgnav.appProxy:setAppDelegate(osgnav)

-- Load config files
require("vrjlua-config")

print("Setting up scenegraph")
navtransform = osg.PositionAttitudeTransform()

print("Setting up run buffer")
runbuf = vrjSync.RunBuffer(nil)

vrjKernel.start()
