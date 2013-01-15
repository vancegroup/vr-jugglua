require("Actions")

--This example allows you to load in a model and have it
--fixed to the wand.  This is usful in simulation mode for
--loading in models and rotating and orienting them with
--the keyboard and mouse for visualization.

device = gadget.PositionInterface("VJWand")

Actions.addFrameAction(function()
		local xform = osg.MatrixTransform()
		m = Model("examples/models/cessna.osg")
		xform:addChild(m)

		RelativeTo.Room:addChild(xform)

		-- Update the cursor position forever.
		while true do
			xform:setMatrix(device.matrix)
			Actions.waitForRedraw()
		end
	end)

