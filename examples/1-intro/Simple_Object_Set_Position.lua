--[[
Simple_Object_Set_Position
This file creates a sphere object (s) and adds to a Transform object (xform)
Then the "xform" transform object is added to the root node (RelativeTo.World)
After running the script we use the node:setPosition() method to change the position of the transform (xform)
]]--
-- 1) Create a transform called "xform" and set its position to (x=0,y=0,z=0)
xform = Transform{
	position={0,0,0},
}
-- 2) Create a Sphere object with radius of .5, and a position at (x=0,y=0,z=0)
s = Sphere{ radius = .5, position = {0,0,0}}

-- 3) Add the sphere (s) to xform
xform:addChild(s)

 -- 4) Add xform to root node, so it becomes visable:
 RelativeTo.World:addChild(xform)
 
 -- 5) Move the xform object with the setPosition method, you can do this inside of your script, or in the VRJuggLua
 --interpreter 
 xform:setPosition(osg.Vec3d(1,0,0))
 -- Try setting the xform to different positions (other than 1,0,0)and see what happens
 -- What happens if you adjust the sphere's location, and then set the position of the xform?