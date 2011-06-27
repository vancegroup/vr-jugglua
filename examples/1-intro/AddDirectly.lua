--[[ EXAMPLE: AddDirectly.lua

Builds on LoadModel.lua so learn that one first.
]]

-- In LoadModel, we loaded the model into a named variable, teapot.
-- This is good if we want to do something with it later, but if we will
-- just be attaching it right away and nothing more, we don't need to name it.

-- This is effectively equivalent, except we don't have a variable named teapot anymore.
RelativeTo.World:addChild(Model("assets/models/teapot.osg"))

