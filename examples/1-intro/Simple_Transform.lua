--[[
Simple_Transform
This file creates an sphere object (s) and adds to a Transform object (xform)
Then the "xform" object is added to the root node (RelativeTo.World)
]]--

-- 1) Create a transform called "xform" and set its position to (x=0,y=0,z=0)
xform = Transform{
	position={0,0,0},
}
-- 2) Create a sphere object to be drawn position (x=0,y=0,z=0), with a radius of .33m.
s = Sphere{position={0,0,0}, radius = .33}

-- 3) Add the sphere object "s" to the transform node "xform", using the 'addChild' method
xform:addChild(s)

-- 4) Add transform ("xform") to root node (RelativeTo.World). 
--This needs to be done in order for the xform object, and all of its children, can be drawn
RelativeTo.World:addChild(xform)

--[[ OPTIONAL: try changing the Transform's position and see what happens to the sphere
You may do this interactively, by loading this script into VRJuggLua then type:
xform:setPosition(osg.Vec3d(x,y,z)) 
where x,y,z are position values.
]]--

