do
	geom = osg.Geometry()
	geode = osg.Geode()

	geode:addDrawable(geom)
	RelativeTo.World:addChild(geode)

	vertices = osg.Vec3Array()
	geom:setVertexArray(vertices)


	colors = osg.Vec4Array()
	geom:setColorArray(colors)
	geom:setColorBinding(osg.Geometry.AttributeBinding.BIND_PER_VERTEX)

	-- Using DrawArrays here because we want to draw contiguous parts of the array.
	linestrip = osg.DrawArrays(osg.PrimitiveSet.Mode.LINE_STRIP)

	geom:addPrimitiveSet(linestrip)
	-- Silly little function to cycle through R G B when called
	getColor = coroutine.wrap(function()
			while true do
				coroutine.yield(osg.Vec4(1, 0, 0, 1))
				coroutine.yield(osg.Vec4(0, 1, 0, 1))
				coroutine.yield(osg.Vec4(0, 0, 1, 1))
			end
		end)

	function addPoint(v)
		vertices.Item:insert(v)
		colors.Item:insert(getColor())
		linestrip:setCount(#(vertices.Item))
	end



end

addPoint(osg.Vec3(1, 1, 1))
addPoint(osg.Vec3(1, 2, 1))
addPoint(osg.Vec3(1, 2, 2))