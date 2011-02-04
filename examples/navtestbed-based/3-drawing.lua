-- This is a "navtestbed" script: open it from the NavTestbed GUI, paste it
-- in the run box of the GUI, or launch it separately using the
-- nav-testbed-launcher script.

require("Actions")
require("TransparentGroup")

pointRadius = 0.0125

device = gadget.PositionInterface("VJWand")

-- This frame action draws and updates our
-- cursor at the device's location.
Actions.addFrameAction(function()
	local xform = osg.MatrixTransform()
	xform:addChild(
		TransparentGroup{
			alpha = 0.7,
			Sphere{
				radius = pointRadius,
				position = {0, 0, 0}
			}
		}
	)

	RelativeTo.Room:addChild(xform)

	-- Update the cursor position forever.
	while true do
		xform:setMatrix(device.matrix)
		Actions.waitForRedraw()
	end
end)

-- This action adds to the scenegraph when you
-- press/hold a button to draw
Actions.addFrameAction(function()
	local drawBtn = gadget.DigitalInterface("VJButton1")
	while true do
		while not drawBtn.pressed do
			Actions.waitForRedraw()
		end

		while drawBtn.pressed do
			local newPoint = osg.PositionAttitudeTransform()
			newPoint:addChild(Sphere{radius = pointRadius, position = {0, 0, 0}})
			newPoint:setPosition(device.position - osgnav.position)

			RelativeTo.World:addChild(newPoint)

			Actions.waitForRedraw()
		end
	end

end)
