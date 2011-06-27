--[[ EXAMPLE: Transformation.lua

Builds on LoadModel.lua so learn that one first.
]]

-- Here we are loading a model, and transforming it to a new position.

-- Note (if you've worked through Group.lua): a transform node is actually
-- a special kind of group node, so we could put multiple children in instead
-- of just the one model.
teapot = Transform{
	position = {-1, 2, -3},
	Model("assets/models/teapot.osg")
}

-- We attach it relative to the world: we can move around it.
RelativeTo.World:addChild(teapot)

