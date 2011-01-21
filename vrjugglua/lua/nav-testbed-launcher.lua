print("osgnav-testbed.lua starting up...")

require("Navigator")
require("Scene")


local setUpActions = false

osgnav = {position = osg.Vec3d(0, 0, 0)}

function osgnav:initScene()
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
	
	dofile(script)
	
end

function osgnav:preFrame()
	self.position = self.position - self.nav:getTranslation(self.appProxy:getTimeDelta(), self.position)
	navtransform:setPosition(self.position)
end

function osgnav:latePreFrame()
	runbuf:runBuffer()
	if Actions then
		if not setUpActions then
			Actions.setAppProxy(osgnav.appProxy)
			setUpActions = true
		end
		Actions.updateFrameActions()
	end
end


print("Setting up run buffer")
runbuf = vrjSync.RunBuffer(nil)

print("App delegate defined, now creating OsgAppProxy")
osgnav.appProxy = vrjApp.OsgAppProxy()
if Actions then
	Actions.setAppProxy(osgnav.appProxy)
end

print ("Now setting OsgAppProxy's delegate")
osgnav.appProxy:setAppDelegate(osgnav)

-- Load config files
--require("vrjlua-config")

print("Parsing arguments")
script = arg[1]

for i=2,#arg do
	vrjKernel.loadConfigFile(arg[i])
end

print("Setting up scenegraph")
navtransform = osg.PositionAttitudeTransform()

print("Setting kernel application")
osgnav.appProxy:setActiveApplication()

print("Entering kernel")
vrjKernel.enter()

