require("AddAppDirectory")
require("StockModels")
AddAppDirectory()

-- add object to scene (to perceive relative navigation)
local teapot = Transform{
	StockModels.Teapot(),
}
RelativeTo.World:addChild(teapot)

--MUST REMOVE STANDARD NAVIGATION BEFORE ADDING CUSTOM NAVIGATION
osgnav.removeStandardNavigation()

-- helper function to translate the world
function translateWorld(x, y, z)
        RelativeTo.World:postMult(osg.Matrixd.translate(x, y, z))
end

-- helper function to get head position in the world
local function getHeadPositionInWorld()
        local head = gadget.PositionInterface("VJHead")
        return RelativeTo.World:getInverseMatrix():preMult(head.position)
end

-- helper function to compute rotation about a point
local function computeRotationAboutPoint(pointVec,rotation)
        local deltaMatrix = osg.Matrixd()
        deltaMatrix:preMult(osg.Matrixd.translate(pointVec))
        deltaMatrix:preMult(osg.Matrixd.rotate(rotation))
        deltaMatrix:preMult(osg.Matrixd.translate(-pointVec))
        return deltaMatrix
end

-- helper function to rotate world about a point
local function rotateWorldAboutPoint(pointVec, rotation)
        local deltamatrix = computeRotationAboutPoint(pointVec,rotation)
        RelativeTo.World:preMult(deltamatrix)
end

-- frame action for basic translation navigation in all directions
Actions.addFrameAction(
	function()
        local translationButton = gadget.DigitalInterface("VJButton0")
        local wand = gadget.PositionInterface('VJWand')
        local rate = 1
        while true do
			repeat
				dt = Actions.waitForRedraw()
			until translationButton.justPressed

			while translationButton.pressed do
				dt = Actions.waitForRedraw()
				local translate_value_x = wand.forwardVector:x() * rate * dt
				local translate_value_y = wand.forwardVector:y() * rate * dt
				local translate_value_z = wand.forwardVector:z() * rate * dt
				translateWorld(-translate_value_x, -translate_value_y, -translate_value_z)
			end
        end
	end
)

-- frame action for wrist deviation rotation
Actions.addFrameAction(
		function()
		local rotateButton = gadget.DigitalInterface("WMButtonDown")
		local wand = gadget.PositionInterface('VJWand')
		local rotRate = 0.5

		local function getWandForwardVectorWithoutY()
			return osg.Vec3d(wand.forwardVector:x(), 0, wand.forwardVector:z())
		end

		while true do
			repeat
				dt = Actions.waitForRedraw()
			until rotateButton.pressed

			local initialWandForwardVector = getWandForwardVectorWithoutY()
			local maximumRotation = osg.Quat()
			local incrementalRotation = osg.Quat()

			while rotateButton.pressed do
				local dt = Actions.waitForRedraw()
				local newForwardVec = getWandForwardVectorWithoutY()
				maximumRotation:makeRotate(newForwardVec, initialWandForwardVector)
				incrementalRotation:slerp(dt * rotRate, osg.Quat(), maximumRotation)
				local newHeadPosition = getHeadPositionInWorld()
				rotateWorldAboutPoint(newHeadPosition, incrementalRotation)
			end
		end
	end
)