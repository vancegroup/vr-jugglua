require("AddAppDirectory")
require("StockModels")
AddAppDirectory()
--must load in a jconf file for the RazarHydra 
-- 	see https://github.com/vancegroup/modular-stock-jconf
-- vrjKernel.loadConfigFile([[INSERT FULL PATH TO RazarHydra.jconf]])

-- add object to scene (to perceive relative navigation)
local teapot = Transform{
	StockModels.Teapot(),
}
RelativeTo.World:addChild(teapot)

--MUST REMOVE STANDARD NAVIGATION BEFORE ADDING CUSTOM NAVIGATION
osgnav.removeStandardNavigation()

-- helper function to get head position in the world
function getHeadPositionInWorld()
	local head = gadget.PositionInterface("VJHead")
	return RelativeTo.World:getInverseMatrix():preMult(head.position)
end

-- helper function used to translate the world
function translateWorld(x, y, z)
	RelativeTo.World:postMult(osg.Matrixd.translate(x, y, z))
end

-- helper function to compute rotation about a point
function computeRotationAboutPoint(pointVec,rotation)
	local deltaMatrix = osg.Matrixd()
	deltaMatrix:preMult(osg.Matrixd.translate(pointVec))
	deltaMatrix:preMult(osg.Matrixd.rotate(rotation))
	deltaMatrix:preMult(osg.Matrixd.translate(-pointVec))
	return deltaMatrix
end

-- helper function to rotate world about a point
function rotateWorldAboutPoint(pointVec, rotation)
	local deltamatrix = computeRotationAboutPoint(pointVec,rotation)
	RelativeTo.World:preMult(deltamatrix)
end

-- hydra translation frame action
Actions.addFrameAction(
	function()
		local leftJoystickY = gadget.AnalogInterface("HydraLeftJSY")
		local rate = 1.5

		local function joystickIsCentered()
			if leftJoystickY.centered > -.05 and leftJoystickY.centered < .05 then
				return true
			else
				return false
			end
		end

		while true do
			repeat
				dt = Actions.waitForRedraw()
			until not joystickIsCentered()

			while not joystickIsCentered() do
				dt = Actions.waitForRedraw()
				local translate_value_z = leftJoystickY.centered * rate * dt
				translateWorld(0, 0, translate_value_z)
			end
		end
	end
)

-- hydra joystick rotation frame action
Actions.addFrameAction(
	function()
		local leftJoystickX = gadget.AnalogInterface("HydraLeftJSX")
		local rotRate = 1

		local function joystickIsCentered()
			if leftJoystickX.centered > -.05 and leftJoystickX.centered < .05 then
				return true
			else
				return false
			end
		end
		
		while true do
			repeat
					dt = Actions.waitForRedraw()
			until not joystickIsCentered()

			local incrementalRotation = osg.Quat()

			while not joystickIsCentered() do
				local angle = leftJoystickX.centered * rotRate * dt
				incrementalRotation:makeRotate(angle, Vec(0, 1, 0))
				local newHeadPosition = getHeadPositionInWorld()
				rotateWorldAboutPoint(newHeadPosition, incrementalRotation)
				local dt = Actions.waitForRedraw()
			end
		end
	end
)