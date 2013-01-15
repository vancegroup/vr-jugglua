-- This is a "navtestbed" script: open it from the NavTestbed GUI, paste it
-- in the run box of the GUI, or launch it separately using the
-- nav-testbed-launcher script.

require("Actions")

sphereRadius = 0.25


local xform = osg.MatrixTransform()
RelativeTo.Room:addChild(xform)
xform:addChild(
	Sphere{
		radius = sphereRadius,
		position = {0, 0, 0}
	}
)

Actions.addFrameAction(
	function()
		local device = gadget.PositionInterface("VJWand")
		while true do
			xform:setMatrix(device.matrix)
			print(device.position)
			Actions.waitForRedraw()
		end

	end
)
