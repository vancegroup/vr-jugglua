#!/home/rpavlik/src/vrjugglua-build/src/vrjlua

print("Loading lua file osgnav.lua")

-- sets the variable "scene"
--loadfile("sceneconfig.lua")
node = osgLua.loadObjectFile('/home/rpavlik/src/wiimote-modelrot/models/cessna.osg')
scene = osg.PositionAttitudeTransform()
scene:addChild(node)
--[[osg.PositionAttitudeTransform()
geode = osg.Geode()
shape = osg.ShapeDrawable()
shape:setShape( osg.Sphere(osg.Vec3(20,20,20),5) )
geode:addDrawable(shape)
scene:addChild(geode)]]--

function vecToString(vec)
	return "(" .. tostring(vec:x()) .. ", " .. tostring(vec:y()) .. ", " .. tostring(vec:z()) .. ")"
end


require("WandNavigator")
require("osgTools")

osgnav = {position = osg.Vec3d(0, 0, 0)}

function osgnav:initScene()
	print("In initScene - setting devices")

	print("Setting up position interface")
	self.wand = gadget.PositionInterface("VJWand")

	print("Setting up digital interface")
	self.button = gadget.DigitalInterface("VJButton0")

	print("Creating navigator")
	self.wandnav = WandNavigator.create(self.wand, self.button, maxspeed)

	print("Setting up scenegraph")
	self.navtransform = osg.PositionAttitudeTransform()
	self.navtransform:addChild(scene)

	print("Attaching to app proxy's scene")
	print("App proxy scene: ")
	osgTools.printInfo(self.appProxy:getScene())
	self.appProxy:getScene():addChild(self.navtransform)
end

function osgnav:preFrame()
	local xlate = self.wandnav:getTranslation(self.appProxy:getTimeDelta(), self.position)
	--print("Translation of me: " .. vecToString(xlate))
	self.position = osgTools.subVec(self.position, xlate)
	--print("Location of the world: " .. vecToString(self.position))

	--showInfo(self.navtransform)
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
vrjKernel.start()

print("Waiting for kernel stop")
vrjKernel.waitForKernelStop()
