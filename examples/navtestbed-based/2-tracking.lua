-- This is a "navtestbed" script: open it from the NavTestbed GUI, paste it
-- in the run box of the GUI, or launch it separately using the 
-- nav-testbed-launcher script.

require("Actions")

createSphere = help.docstring[[
	Adds a sphere to the scene at the location of the tracker device
	you name.  You might try "VJWand" or "VJHead", to get started.
	
	You can also pass a radius for the sphere, in meters.
]] .. function (deviceName, rad)
	local xform = osg.MatrixTransform()
	navtransform:addChild(xform)
	xform:addChild(
		Sphere{
			radius = rad or 0.0125,
			position = {0, 0, 0}
		}
	)

	Actions.addFrameAction(function()
		local device = gadget.PositionInterface(deviceName or "VJHead")
		while true do
			xform:setMatrix(device.matrix)
			print(device.position)
			Actions.waitForRedraw()
		end

	end)
end
