--[[
EXAMPLE: Transformation.lua
PREREQUISITE: LoadModel.lua
DESCRIPTION: Here we are loading a model, placing it in a transform node - so we can move it
			 to a new position.
			 (Note, if you've worked through example Group.lua, a transform node is actually
			 a special kind of group node, so we could put multiple children in instead
			 of just the one model.)
]]

-- We create a transform and call it "teapot", this allows us to work with it later
teapot = Transform{
	position = {-1, 2, -3},
	Model("assets/models/teapot.osg")
}

-- We attach it relative to the world: we can move around it.
RelativeTo.World:addChild(teapot)

