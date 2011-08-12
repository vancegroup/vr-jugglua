--[[
Simple_Object
This file creates an sphere object (obj) and adds it as a child to the root node (RelativeTo.World)
]]--

-- 1) Create a Sphere* object with radius of .5, and a position at (x=1,y=0,z=0)
obj = Sphere{ radius = .5, position = {1,0,0}}
-- 2) Add obj (sphere) to the root node so that it will become visible
RelativeTo.World:addChild(obj)

--*  A sphere is one of several pre-defined objects. Because it is pre-defined in another 
--    Lua script we are able to create it with simple syntax ("Sphere{...").
-- We are also able to load models in the same fashion: "StockModels.Teapot()" - loads a model of a teapot
