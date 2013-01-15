--[[
Simple_Object_Orientation
This file creates a model object (m) and adds to a Transform object (xform)
We set the orientation of the transform, which by extension, sets the orientation of all of its children (m)
Then the "xform" transform object is added to the root node (RelativeTo.World)
]]--

-- 1) Create a transform called "xform" and set its position to (x=0,y=0,z=0)
xform = Transform{
	position = {0, 0, 0},
	-- set the transform orientation to -90 on the x (first element of Axis values)
	orientation = AngleAxis(Degrees(-90), Axis{1.0, 0.0, 0.0}),
}
-- 1)import a model:
m = Model("examples/models/cessna.osg")

-- 3) Add the model object "m" to the transform node "xform", using the 'addChild' method, the orientation
--will be inherted by the children of xform 
xform:addChild(m)

-- 4) Add transform ("xform") to root node (RelativeTo.World). 
--This needs to be done in order for the xform object, and all of its children, can be drawn
RelativeTo.World:addChild(xform)

--[[ 
OPTIONAL: 
try changing the Transform's orientation by adjusting the "Axis" and "Degree" values
values and see what happens to the model
]]--