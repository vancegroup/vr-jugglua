do
	local function addPointToLine(line, point, color)
		line.vertices.Item:insert(point)
		line.colors.Item:insert(color)
		print(#(line.vertices.Item), ": Added point ", point, " in color ", color)
		line.linestrip:setCount(#(line.vertices.Item))
	end
	local function addLineToGeode(line, geode)
		geode:addDrawable(line.geom)
	end
	local LineMT = {
		__index = {
			addToGeode = addLineToGeode,
			addPoint = addPointToLine
		}
	}
	local function createLine()
		local line = {
			geom = osg.Geometry(),
			vertices = osg.Vec3Array(),
			colors = osg.Vec4Array(),
			linestrip = osg.DrawArrays(osg.PrimitiveSet.Mode.LINE_STRIP),
		}
		line.geom:setVertexArray(line.vertices)
		line.geom:setColorArray(line.colors)
		line.geom:setColorBinding(osg.Geometry.AttributeBinding.BIND_PER_VERTEX)
		line.geom:addPrimitiveSet(line.linestrip)
		setmetatable(line, LineMT)
		return line
	end


	Line = setmetatable({}, {__call = createLine} )
end