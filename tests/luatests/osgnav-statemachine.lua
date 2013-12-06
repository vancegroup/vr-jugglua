require("Navigator")
StateMachine = require("StateMachine")

print("CTEST_FULL_OUTPUT")

-- Define application-wide data --
function myinit()
	print("Setting up scenegraph")
	navtransform = osg.PositionAttitudeTransform()
	--navtransform:addChild(osgLua.loadObjectFile('cessna.osg'))

	print("Attaching to scene")
	StateMachine.getScene():addChild(navtransform)
end

---------------------- osgnav state ----------------------------

-- Define the osgnav "state" --
osgnav = {}
osgnav.position = osg.Vec3d(0, 0, 0)

-- When entering "osgnav"
function osgnav:enter()
	-- Set up the state
	print("Setting up position interface")
	local wand = gadget.PositionInterface("VJWand")

	print("Setting up digital interface")
	local button = gadget.DigitalInterface("VJButton0")
	
	local button2 = gadget.DigitalInterface("VJButton1")

	print("Creating navigator")
	self.nav = Navigator.create(maxspeed)
	Navigator.useWandTranslation(self.nav, wand, button)

	os.exit(0)
	
	
	-- Set up the events
	self.events = {
		[function() return button2.justPressed end] = StateMachine.createStateTransition(simplerotation);
	}
end

-- When updating "osgnav"
function osgnav:update(dt)
	self.position = self.position - self.nav:getTranslation(dt, self.position)
	navtransform:setPosition(self.position)
end

-- osgnav:leave is undefined - no specific action we wish to carry out.


----------- simplerotation state --------------

-- Define the simplerotation "state"
simplerotation = {}
simplerotation.degrees = 0

-- When entering "simplerotation"
function simplerotation:enter()
	local button = gadget.DigitalInterface("VJButton2")
	self.events = {
		[function() return button.justPressed end] = StateMachine.createStateTransition(osgnav);
	}
end

-- When updating "simplerotation"
function simplerotation:update(dt)
	degreesPerSec = 3
	simplerotation.degrees = simplerotation.degrees + degreesPerSec * dt
	navtransform:setAttitude(osg.Quat(simplerotation.degrees,  osg.Vec3d(0, 1, 0))	)
end

-- simplerotation:leave is undefined - no specific action we wish to carry out.

------------------ Start up the app -----------------------

print("Loading config files into kernel")
vrjKernel.loadConfigFile("standalone.jconf")

StateMachine.setInitFunction(myinit)
StateMachine.setStartingState(osgnav)
StateMachine.runApp()
