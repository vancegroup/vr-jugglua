--[[
Simple_Model
This file loads a model and  adds it as a child to the root node (RelativeTo.World)
]]--

-- 1)import a model:
obj = Model("examples/model/cessna.osg")

-- 2) Add model to the root node (so that it will become visible in the scene)
RelativeTo.World:addChild(obj)
