print("osgnav-testbed.lua starting up...")

require("Navigator")
require("osgTools")
require("Scene")

osgnav = {position = osg.Vec3d(0, 0, 0)}

function osgnav:initScene()
	runbuf:init()

	--print("Setting up position interface")
	local wand = gadget.PositionInterface("VJWand")

	--print("Setting up digital interface")
	local button = gadget.DigitalInterface("VJButton0")

	--print("Creating navigator")
	self.nav = Navigator.create(maxspeed)
	Navigator.useWandTranslation(self.nav, wand, button)

	--print("Setting up scenegraph")
	navtransform = osg.PositionAttitudeTransform()

	--print("Attaching to app proxy's scene")
	self.appProxy:getScene():addChild(navtransform)
	
	print("Scenegraph Navigation Testbed loaded!")
	print("")
	print("navtransform is your root-level group node,")
	print("do navtransform:addChild(somenode) to attach 'somenode' to the scene graph")
	print("")
	print("Simulator Mode Instructions for Navigation --");
	print("In the simulator window:");
	print("   Hold 'shift' and move the mouse to rotate the simulated wand");
	print("   Click to move in the direction of the wand's tip");
	print("");
end

function osgnav:preFrame()
	self.position = osgTools.subVec(self.position, self.nav:getTranslation(self.appProxy:getTimeDelta(), self.position))
	navtransform:setPosition(self.position)
end

function osgnav:latePreFrame()
	runbuf:runBuffer()
end


print("Setting up run buffer")
runbuf = vrjSync.RunBuffer(nil)

print("App delegate defined, now creating OsgAppProxy")
osgnav.appProxy = vrjApp.OsgAppProxy()

print ("Now setting OsgAppProxy's delegate")
osgnav.appProxy:setAppDelegate(osgnav)

-- Load config files
require("vrjlua-config")

print("Setting up scenegraph")
navtransform = osg.PositionAttitudeTransform()

print("Setting kernel application")
osgnav.appProxy:setActiveApplication()


