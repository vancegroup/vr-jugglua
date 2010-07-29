maxspeed = 150

require("osgTools")

local function create(max_vel, max_rot_vel, fwd_slope, back_slope)
	if max_rot_vel == nil then
		-- radians per second
		max_rot_vel = 0.5
		print("Using default max_rot_vel of " .. tostring(max_rot_vel))
	end
	
	if fwd_slope == nil then
		fwd_slope = 15
		print("Using default forward-vel slope of " .. tostring(fwd_slope))
	end
	
	if back_slope == nil then
		back_slope = 15
		print("Using default backward-vel slope of " .. tostring(back_slope))
	end
	
	local nav = {}
	nav.vel = 0
	nav.goal_vel = 0
	nav.max_vel = max_vel
	nav.position = osg.Vec3d(0.0, 0.0, 0.0)
	
	nav.rot_vel = 0
	nav.goal_rot_vel = 0
	nav.max_rot_vel = max_rot_vel 
	nav.rot_axis = osg.Vec3d(0.0, 1.0, 0.0)
	nav.rot = 0
	
	nav.pose = osg.Matrixd()
	
	function nav:getTranslation(dt)
		-- update goal velocity
		self:updateGoalVel()
		
		-- Clamp dt for safety
		if dt > 1.0 then dt = 1.0 end
		
		-- handle transitions from negative to positive
		local flip
		if self.goal_vel < -0 then
			flip = true
			self.vel = - self.vel
			self.goal_vel = - self.goal_vel
		else
			flip = false
		end

		-- If we aren't at our goal velocity, increase by 1/15th
		-- or decrease by 1/15th our max velocity, per second
		if self.vel < self.goal_vel then
			self.vel = self.vel + (self.max_vel/fwd_slope) * dt
			-- Enforce the max velocity
			if self.vel > self.goal_vel then self.vel = self.goal_vel end

		elseif self.vel > self.goal_vel then
			self.vel = self.vel - (self.max_vel/back_slope) * dt
			-- Enforce the minimum velocity
			if self.vel < self.goal_vel then self.vel = self.goal_vel end
		end

		-- handle transitions from negative to positive
		if flip then
			self.vel = - self.vel
			self.goal_vel = - self.goal_vel
		end
		
		--print("WandNavigator: Velocity is now " .. self.vel)

		-- Scale the velocity by time and multiply by the direction
		-- of the wand
		local mvmt = dt * self.vel
		--print("Movement magnitude: " .. mvmt)
		local fwd = self:getTransVector()
		return osgTools.scaleVec(fwd, mvmt)
	end
	
	function nav:getRotation(dt)
		-- update goal velocity
		self:updateGoalRotVel()
		
		-- Clamp dt for safety
		if dt > 1.0 then dt = 1.0 end
		
		-- handle transitions from negative to positive
		local flip
		if self.goal_rot_vel < -0 then
			flip = true
			self.rot_vel = - self.rot_vel
			self.goal_rot_vel = - self.goal_rot_vel
		else
			flip = false
		end

		-- If we aren't at our goal velocity, change by 1/15th
		-- of our max velocity, per second
		if self.rot_vel < self.goal_rot_vel then
			self.rot_vel = self.vel + (self.max_rot_vel/fwd_slope) * dt
			-- Enforce the max velocity
			if self.rot_vel > self.goal_rot_vel then self.rot_vel = self.goal_rot_vel end

		elseif self.rot_vel > self.goal_rot_vel then
			self.rot_vel = self.rot_vel - (self.max_rot_vel/back_slope) * dt
			-- Enforce the minimum velocity
			if self.rot_vel < self.goal_rot_vel then self.rot_vel = self.goal_rot_vel end
		end
		
		-- handle transitions from negative to positive
		if flip then
			self.rot_vel = - self.rot_vel
			self.goal_rot_vel = - self.goal_rot_vel
		end

		--print("WandNavigator: Rotational velocity is now " .. self.rot_vel)

		-- Scale the velocity by time
		return dt * self.rot_vel
	end
	
	function nav:setWorldRelativeTransform(xform, dt)
		local delta = osg.Matrixd()
		if self.getTranslation ~= nil and self.updateGoalVel ~= nil then
			delta:setTrans(osgTools.subVec(osg.Vec3d(0.0, 0.0, 0.0), self:getTranslation(dt)))
			-- self.position = osgTools.subVec(self.position, )
		end
		
		if self.getRotation ~= nil and self.updateGoalRotVel ~= nil then
			local dRotate = - self:getRotation(dt)
			delta:setRotate(osg.Quat(dRotate, self.rot_axis))
			--self.rot = self.rot - dRotate
		end
		
		--nav.pose:preMult(delta)
		
		--xform:setAttitude(osg.Quat(self.rot,  self.rot_axis))
		--xform:setPosition(self.position)
		xform:preMult(delta)
		
	end
	return nav
end


local function useWandTranslation(nav, wand, button)
	function nav:getTransVector()
		local fwd = osg.Vec3d()
		fwd:set(wand:getForwardVector())	
		fwd:normalize()
		return fwd
	end
	function nav:updateGoalVel()
		if button:isPressed() == true then
			-- Set our goal velocity
			self.goal_vel = self.max_vel
		else
			-- Slow down
			self.goal_vel = 0
		end
	end
end

local function useGamepadTranslation(nav, axisX, axisY, axisZ)
	function nav:getTransVector()
		local fwd = osg.Vec3d(axisX.getCentered(), axisY.getCentered, axisZ.getCentered)
		fwd:normalize()
		return fwd
	end
	
	function nav:updateGoalVel()
		local L = self:getTransVector():length()
		if L > 1.0 then
			-- Cap the velocity
			self.goal_vel = self.max_vel
		else
			-- Analog control
			self.goal_vel = L * self.max_vel
		end
	end
end


Navigator = {
	create = create,
	useWandTranslation = useWandTranslation,
	useGamepadTranslation = useGamepadTranslation}

return Navigator
