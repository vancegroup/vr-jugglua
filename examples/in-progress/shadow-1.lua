
-- Based on http://www.openscenegraph.org/projects/osg/wiki/Support/ProgrammingGuide/osgShadow

require("StockModels")

require("osgShadow")

scene = osgShadow.ShadowedScene()

ReceivesShadowTraversalMask = 0x1
CastsShadowTraversalMask = 0x2

scene:setReceivesShadowTraversalMask(ReceivesShadowTraversalMask)
scene:setCastsShadowTraversalMask(CastsShadowTraversalMask)

sm = osgShadow.ShadowMap()

mapres = 1024
sm:setTextureSize(osg.Vec2s(mapres, mapres))
sm:setTextureUnit(1)

scene:setShadowTechnique(sm)


newroom = Transform{
	scale = ScaleFrom.inches,
	position = {0, -2.5, 0},
	Model("examples/models/wavyroom.ive")
}
newroom:setNodeMask(ReceivesShadowTraversalMask)



teapot = Transform{
	position = {0, 1, 0},
	StockModels.Teapot()
}
teapot:setNodeMask(CastsShadowTraversalMask)


scene:addChild(newroom)
scene:addChild(teapot)

RelativeTo.World:addChild(scene)
RelativeTo.Room:addChild(
	Lighting{
		number = 0,
		ambient = 0.9,
		diffuse = 0.7,
		specular = 0.5,
		position = {0, 3, 2},
		positional = true
	}
)

