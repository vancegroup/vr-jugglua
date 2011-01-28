require("Actions")
pointRadius = 0.0125

function makeTransparent(node, alpha)
	local state = node:getOrCreateStateSet()
	state:setRenderingHint(2) -- transparent bin

	local CONSTANT_ALPHA = 0x8003
	local ONE_MINUS_CONSTANT_ALPHA = 0x8004
	local bf = osg.BlendFunc()
	bf:setFunction(CONSTANT_ALPHA, ONE_MINUS_CONSTANT_ALPHA)
	state:setAttributeAndModes(bf)

	local bc = osg.BlendColor(osg.Vec4(1.0, 1.0, 1.0, alpha or 0.5))
	state:setAttributeAndModes(bc)
	node:setStateSet(state)
end
root = osgnav.appProxy:getScene()



local device = gadget.PositionInterface("VJWand")

Actions.addFrameAction(function()
	local xform = osg.MatrixTransform()
	xform:addChild(Sphere{
		radius = pointRadius,
		position = {0, 0, 0}
	})
	makeTransparent(xform, 0.7)

	root:addChild(xform)

	while true do
		xform:setMatrix(device.matrix)
		Actions.waitForRedraw()
	end
end)
	
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
			
			navtransform:addChild(newPoint)
			
			Actions.waitForRedraw()
		end
	end

end)
