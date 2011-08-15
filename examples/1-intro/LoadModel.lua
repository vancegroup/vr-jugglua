--[[
Simple_Model
This file loads a model and  adds it as a child to the root node (RelativeTo.World)
]]--

-- Here we are loading a model. Don't worry about where the model file is located right now.
teapot = Model("assets/models/teapot.osg")

-- We must attach it to a parent node in the scene.  For now, we'll just always use RelativeTo.World.
-- When you want to know more, see the RelativeTo tutorial.
RelativeTo.World:addChild(teapot)

