geom = osg.Geometry()
geode = osg.Geode()

vertices = osg.Vec3Array()
vertices.Item:insert(osg.Vec3(1, 0, 0))
-- this also works: vertices.Item[1] = osg.Vec3(1, 0, 0)

vertices.Item:insert(osg.Vec3(1, 1, 0))
vertices.Item:insert(osg.Vec3(2, .5, 0))

geom:setVertexArray(vertices)

linestrip = osg.DrawElementsUInt(osg.PrimitiveSet.Mode.TRIANGLES, 0)
-- counter-clockwise order
linestrip.Item:insert(0)
linestrip.Item:insert(1)
linestrip.Item:insert(2)


geom:addPrimitiveSet(linestrip)

colors = osg.Vec4Array()
colors.Item:insert(osg.Vec4(1, 0, 0, 1))
colors.Item:insert(osg.Vec4(0, 0, 1, 1))
colors.Item:insert(osg.Vec4(1, 1, 1, 1))
geom:setColorArray(colors)
geom:setColorBinding(osg.Geometry.AttributeBinding.BIND_PER_VERTEX)
geode:addDrawable(geom)
RelativeTo.World:addChild(geode)
