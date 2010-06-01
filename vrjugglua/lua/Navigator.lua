maxspeed = 150

require("osgTools")

local function create(max_vel)
	local nav = {}
	nav.vel = 0
	nav.goal_vel = 0
	nav.max_vel = max_vel
	function nav:getTranslation(dt, prev)
		-- update goal velocity
		self:updateGoalVel()
		
		-- Clamp dt for safety
		if dt > 1.0 then dt = 1.0 end

		-- If we aren't at our goal velocity, increase by 1/30th
		-- or decrease by 1/10th our max velocity, per second
		if self.vel < self.goal_vel then
			self.vel = self.vel + (self.max_vel/30) * dt
			-- Enforce the max velocity
			if self.vel > self.goal_vel then self.vel = self.goal_vel end

		elseif self.vel > self.goal_vel then
			self.vel = self.vel - (self.max_vel/10) * dt
			-- Enforce the minimum velocity
			if self.vel < self.goal_vel then self.vel = self.goal_vel end
		end

		--print("WandNavigator: Velocity is now " .. self.vel)

		-- Scale the velocity by time and multiply by the direction
		-- of the wand
		local mvmt = dt * self.vel
		--print("Movement magnitude: " .. mvmt)
		local fwd = self:getTransVector()
		return osgTools.scaleVec(fwd, mvmt)
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
