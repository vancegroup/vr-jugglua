#!/home/rpavlik/src/vrjugglua-build/src/vrjlua

print("Loading lua file osgnav.lua")

maxspeed = 150

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

function addVec3d(a, b)
	--print("In addVec3d")
	return osg.Vec3d(a:x() + b:x(),
		a:y() + b:y(),
		a:z() + b:z())
end

function subtractVec3d(a, b)
	--print("In subtractVec3d")
	return osg.Vec3d(a:x() - b:x(),
		a:y() - b:y(),
		a:z() - b:z())
end

function scaleVec3d(a, scale)
	--print("in scaleVec3d")
	return osg.Vec3d(a:x() * scale,
		a:y() * scale,
		a:z() * scale)
end

function showInfo(instance)
	local c = osgLua.getTypeInfo(instance)
	if c then
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

function vecToString(vec)
	return "(" .. tostring(vec:x()) .. ", " .. tostring(vec:y()) .. ", " .. tostring(vec:z()) .. ")"
end


function createWandNavigator(wand, button, max_vel)
	return {
		vel = 0,
		goal_vel = 0,
		getTranslation = function(self, dt, prev)
			if button:isPressed() == true then
				-- Set our goal velocity
				self.goal_vel = max_vel
			else
				-- Slow down
				self.goal_vel = 0
			end


			-- If we aren't at our goal velocity, increase by 1/30th
			-- or decrease by 1/10th our max velocity, per second
			if self.vel < self.goal_vel then
				self.vel = self.vel + (max_vel/30) * dt
				-- Enforce the max velocity
				if self.vel > self.goal_vel then self.vel = self.goal_vel end

			elseif self.vel > self.goal_vel then
				self.vel = self.vel - (max_vel/10) * dt
				-- Enforce the minimum velocity
				if self.vel < self.goal_vel then self.vel = self.goal_vel end
			end

			--print("WandNavigator: Velocity is now " .. self.vel)

			-- Scale the velocity by time and multiply by the direction
			-- of the wand
			local mvmt = dt * self.vel
			--print("Movement magnitude: " .. mvmt)
			local fwd = osg.Vec3d()
			fwd:set(wand:getForwardVector())
			--print(vecToString(fwd))
			--showInfo(fwd)
			fwd:normalize()
			return scaleVec3d(fwd, mvmt)
		end
	}
end


osgnav = {position = osg.Vec3d(0, 0, 0)}

function osgnav:initScene()
	print("In initScene - setting devices")

	print("Setting up position interface")
	self.wand = gadget.PositionInterface("VJWand")

	print("Setting up digital interface")
	self.button = gadget.DigitalInterface("VJButton0")

	print("Creating navigator")
	self.wandnav = createWandNavigator(self.wand, self.button, maxspeed)

	print("Setting up scenegraph")
	self.navtransform = osg.PositionAttitudeTransform()
	self.navtransform:addChild(scene)

	print("Attaching to app proxy's scene")
	print("App proxy scene: ")
	showInfo(self.appProxy:getScene())
	self.appProxy:getScene():addChild(self.navtransform)
end

function osgnav:preFrame()
	--print("Location of the world: " .. vecToString(self.position))
	--print("Delta: " .. self.appProxy:getTimeDelta())
	--showInfo(self.position)
	local xlate = self.wandnav:getTranslation(self.appProxy:getTimeDelta(), self.position)
	--print("Translation of me: " .. vecToString(xlate))
	self.position = subtractVec3d(self.position, xlate)
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
