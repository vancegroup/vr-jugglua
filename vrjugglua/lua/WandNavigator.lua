maxspeed = 150

require("osgTools")

local function create(wand, button, max_vel)
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
			return osgTools.scaleVec(fwd, mvmt)
		end
	}
end

WandNavigator = {
	create = create}

return WandNavigator
