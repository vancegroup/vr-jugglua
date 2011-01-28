require("Actions")

function createSphere(deviceName)
	local xform = osg.MatrixTransform()
	navtransform:addChild(xform)
	xform:addChild(Sphere{radius = 0.0125, position = {0, 0, 0}})

	Actions.addFrameAction(function()
		local device = gadget.PositionInterface(deviceName or "VJHead")
		while true do
			xform:setMatrix(device.matrix)
			print(device.position)
			Actions.waitForRedraw()
		end

	end)
end