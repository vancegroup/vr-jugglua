require("getScriptFilename")

fn = getScriptFilename()
assert(fn, "Have to load this from file, not copy and paste, or we can't find our models!")
vrjLua.appendToModelSearchPath(fn)
vrjLua.appendToLuaRequirePath(fn)

require("Actions")
require("TransparentGroup")
dofile(vrjLua.findInModelSearchPath("Line.lua"))

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

-- Silly little function to cycle through R G B when called
getColor = coroutine.wrap(function()
	while true do
		coroutine.yield(osg.Vec4(1, 0, 0, 1))
		coroutine.yield(osg.Vec4(0, 1, 0, 1))
		coroutine.yield(osg.Vec4(0, 0, 1, 1))
	end
end)
--[[
do
	local function addPointToLine(line, point, color)
		line.vertices.Item:insert(point)
		line.colors.Item:insert(color)
		print(#(line.vertices.Item), ": Added point ", point, " in color ", color)
		line.linestrip:setCount(#(line.vertices.Item))
	end
	local LineMT = {
		__index = {
			addToGeode = addLineToGeode,
			addPoint = addPointToLine
		}
	}
	local function createLine()
		local line = {
			node = osg.Geode(),
			geom = osg.Geometry(),
			vertices = osg.Vec3dArray(),
			colors = osg.Vec4Array(),
			linestrip = osg.DrawArrays(osg.PrimitiveSet.Mode.LINE_STRIP),
		}
		line.geom:setVertexArray(line.vertices)
		line.geom:setColorArray(line.colors)
		line.geom:setColorBinding(osg.Geometry.AttributeBinding.BIND_PER_VERTEX)
		line.geom:addPrimitiveSet(line.linestrip)
		line.node:addDrawable(line.geom)
		setmetatable(line, LineMT)
		return line
	end


	Line = setmetatable({}, {__call = createLine} )
end
]]

-- This action adds to the scenegraph when you
-- press/hold a button to draw
Actions.addFrameAction(function()
	local drawBtn = gadget.DigitalInterface("VJButton1")

	--local lines = {}
	while true do
		while not drawBtn.pressed do
			Actions.waitForRedraw()
		end

		-- OK, starting a new line.
		print("Starting a new line")
		local line = Line()
		local geode = osg.Geode()
		line:addToGeode(geode)

		--table.insert(lines, line)
		--RelativeTo.World:addChild(line.node)
		RelativeTo.World:addChild(geode)
		while drawBtn.pressed do
			line:addPoint(device.position - osgnav.position,osg.Vec4(1,1,1,1)--[[ getColor()]])
			Actions.waitForRedraw()
		end
	end

end)
