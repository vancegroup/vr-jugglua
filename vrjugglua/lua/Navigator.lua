maxspeed = 5

local function createEaseInOut(max_vel, seconds_to_max, seconds_to_stop)
	local ease = {
		max = max_vel,
		vel = 0,
		--		seconds_to_max = seconds_to_max,
		--	seconds_to_stop = seconds_to_stop,
		acceleration = max_vel / seconds_to_max,
		deceleration = max_vel / seconds_to_stop }

	function ease:getUpdatedVelocity(goal_vel, dt)
		-- Clamp dt for safety
		if dt > 1.0 then
			dt = 1.0
		end

		local vel = self.vel
		-- handle transitions from negative to positive: goal should
		-- always be positive
		local flip
		if goal_vel < -0 then
			flip = true
			vel = - vel
			goal_vel = - goal_vel
		else
			flip = false
		end

		-- If we aren't at our goal velocity..., increase by 1/15th
		-- or decrease by 1/15th our max velocity, per second
		if vel < goal_vel then
			-- because we're too low:
			vel = vel + (self.acceleration) * dt
			-- Don't overshoot
			if vel > goal_vel then
				vel = goal_vel
			end

		elseif vel > goal_vel then
			-- because we're too high:
			vel = vel - (self.deceleration) * dt
			-- Don't overshoot
			if vel < goal_vel then
				vel = goal_vel
			end
		end

		-- flip back if we flipped before the ramping
		if flip then
			vel = - vel
		end

		self.vel = vel

		return self.vel
	end

	return ease
end

local function create(max_vel, max_rot_vel, seconds_to_max, seconds_to_stop)
	if max_rot_vel == nil then
		-- radians per second
		max_rot_vel = 0.5
		print("Using default max_rot_vel of " .. tostring(max_rot_vel))
	end

	if seconds_to_max == nil then
		seconds_to_max = 3
		print("Using default seconds_to_max of " .. tostring(seconds_to_max))
	end

	if seconds_to_stop == nil then
		seconds_to_stop = 1
		print("Using default seconds_to_stop slope of " .. tostring(seconds_to_stop))
	end

	local nav = {}
	nav.max_vel = max_vel

	nav.max_rot_vel = max_rot_vel
	nav.rot_axis = osg.Vec3d(0.0, 1.0, 0.0)

	nav.translation = {
		vel = 0.0,
		max_vel = max_vel }
	nav.translation.ease = createEaseInOut(max_vel, seconds_to_max, seconds_to_stop)

	nav.rotation = {
		vel = 0.0,
		max_vel = max_rot_vel }
	nav.rotation.ease = createEaseInOut(max_rot_vel, seconds_to_max, seconds_to_stop)

	function nav:getTranslation(dt)
		-- get the new velocity
		local vel = self.translation.ease:getUpdatedVelocity(self:getGoalVel(), dt)

		-- Scale the velocity by time and multiply by the direction
		-- of the wand
		local mvmt = dt * vel
		--print("Movement magnitude: " .. mvmt)
		local fwd = self:getTransVector()
		return fwd * mvmt
	end

	function nav:getRotation(dt)
		-- get the new velocity
		local vel = self.rotation.ease:getUpdatedVelocity(self:getGoalRotVel(), dt)

		-- Scale the velocity by time
		return dt * vel
	end

	function nav:setWorldRelativeTransform(xform, dt)
		local delta = osg.Matrixd()

		local dXlate = self:getTranslation(dt)
		delta:setTrans( - dXlate)


		local dRotate = - self:getRotation(dt)
		delta:setRotate(osg.Quat(dRotate, self.rot_axis))

		--nav.pose:preMult(delta)

		--xform:setAttitude(osg.Quat(self.rot,  self.rot_axis))
		--xform:setPosition(self.position)
		xform:postMult(delta)

	end
	return nav
end


local function useWandTranslation(nav, wand, button)
	function nav:getTransVector()
		local fwd = osg.Vec3d()
		fwd:set(wand.forwardVector)
		fwd:normalize()
		return fwd
	end
	function nav:getGoalVel()
		if button.pressed then
			-- Set our goal velocity
			return self.max_vel
		else
			-- Slow down
			return 0
		end
	end
end

local function useGamepadTranslation(nav, axisX, axisY, axisZ)
	function nav:getTransVector()
		local fwd = osg.Vec3d(axisX.centered, axisY.centered, axisZ.centered)
		fwd:normalize()
		return fwd
	end

	function nav:getGoalVel()
		local L = self:getTransVector():length()
		if L > 1.0 then
			-- Cap the velocity
			return self.max_vel
		else
			-- Analog control
			return L * self.max_vel
		end
	end
end


Navigator = {
	create = create,
	useWandTranslation = useWandTranslation,
	useGamepadTranslation = useGamepadTranslation}

return Navigator
