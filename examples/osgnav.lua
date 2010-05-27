#!/home/rpavlik/src/vrjugglua-build/src/vrjlua

print("Loading lua file osgnav.lua")

-- sets the variable "scene"
--loadfile("sceneconfig.lua")
scene = osg.PositionAttitudeTransform()
geode = osg.Geode()
shape = osg.ShapeDrawable()
shape:setShape( osg.Sphere(osg.Vec3(0,0,0),15) )
geode:addDrawable(shape)
scene:addChild(geode)

function showInfo(instance)
	local c = osgLua.getTypeInfo(instance)
	if c  then
		print("name = ",c.name)
		for i,v in ipairs(c.constructors) do
	  	 print(string.format("\tconstructor(%2d) %s",i,v))
		end
		for i,v in ipairs(c.methods) do
	  	 print(string.format("\tmethod(%2d) %s",i,v))
		end
	else
		print(instance.." ** not found **")
	end
end


function createWandNavigator(wand, button, max_vel)
	return {
		vel = 0,
		goal_vel = 0,
		getPosition = function(self, dt, prev)
			print("WandNavigator: Checking button")
			if button:isPressed() == true then
				-- Set our goal velocity
				self.goal_vel = self.max_vel
			else
				-- Slow down
				self.goal_vel = 0
			end

			print("WandNavigator: Updating velocity")
			-- If we aren't at our goal velocity, increase by 1/30th
			-- or decrease by 1/10th our max velocity, per second
			if self.vel < self.goal_vel then
				self.vel = self.vel + (self.goal_vel/30) * dt
				-- Enforce the max velocity
				if self.vel > self.goal_vel then self.vel = self.goal_vel end

			elseif self.vel > self.goal_vel then
				self.vel = self.vel - (self.goal_vel/10) * dt
				-- Enforce the minimum velocity
				if self.vel < self.goal_vel then self.vel = self.goal_vel end
			end

			-- Scale the velocity by time and multiply by the direction
			-- of the wand
			print("WandNavigator: computing translation magnitude")
			local mvmt = dt * self.vel

			print("WandNavigator: getting wand forward vector")
			showInfo(wand:getForwardVector())
			local fwd = osg.Vec3()
			fwd:set(wand:getForwardVector())
			local magnitude = fwd:length()
			showInfo(fwd)


			local xlate = osg.Vec3(
				prev:x() - (fwd:x() / magnitude) * mvmt,
				prev:y() - (fwd:y() / magnitude) * mvmt,
				prev:z() - (fwd:z() / magnitude) * mvmt)
			print("WandNavigator: vector is " .. tostring(xlate:x()) .. ", " .. tostring(xlate:y()) .. ", " .. tostring(xlate:z()))
			return xlate
		end
	}
end


osgnav = {
	position = osg.Vec3(0, 0, 0),

	initScene = function (self)
		print("In initScene - setting devices")

		print("Setting up position interface")
		self.wand = gadget.PositionInterface("VJWand")

		print("Setting up digital interface")
		self.button = gadget.DigitalInterface("VJButton0")

		print("Creating navigator")
		self.wandnav = createWandNavigator(self.wand, self.button, 1.5)

		print("Setting up scenegraph")
		self.navtransform = osg.PositionAttitudeTransform()
		self.navtransform:addChild(scene)

		print("Attaching to app proxy's scene")
		print("App proxy scene: ")
		showInfo(self.appProxy:getScene())
		self.appProxy:getScene():addChild(self.navtransform)
	end,

	preFrame = function (self)
		print("In preFrame")
		local xlate

		print("Getting translation from navigator")
		self.position:set(self.wandnav:getPosition(self.appProxy:getTimeDelta(), self.position))
	end
}


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