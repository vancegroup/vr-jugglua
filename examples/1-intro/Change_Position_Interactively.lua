--[[
Change Position Interactively.
This file creates a sphere object (s) and adds to a Transform object (xform)
Then the "xform" transform object is added to the root node (RelativeTo.World)
After running the script we use the node:setPosition() method to change the position of the transform (xform) interactively. 
This means we do not have to re-run the script everytime we make a change to our code.
]]--
-- 1) Create a transform called "xform" and set its position to (x=0,y=0,z=0)
xform = Transform{
	position = {0, 0, 0},
}
-- 2) Create a Sphere object with radius of .5, and a position at (x=0,y=0,z=0)
s = Sphere{ radius = .5, position = {0, 0, 0}}

-- 3) Add the sphere (s) to xform
xform:addChild(s)

-- 4) Add xform to root node, so it becomes visible:
RelativeTo.World:addChild(xform)

-- 5) Run this script in VRJuggLua.

-- 6) Once the script is running, move the xform object with the setPosition method.
-- Enter the following command into the VRJuggLua Scenegraph Navigation Testbed Window and hit "Run"
-- xform:setPosition(Vec(1, 0, 0))

--[[
You can enter commands into the VRJuggLua Scenegraph Navigation Testbed Window interactively. 
This means you do not have to re-run the script everytime you make a change. If enter a change into 
the Testbed Window you like, make sure to include it in your script.
]]--

