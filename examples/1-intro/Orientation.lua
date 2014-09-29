--[[ 
EXAMPLE: Orientation.lua
PREREQUISITE: Transformation.lua
DESCRIPTION: Here we are loading a model, placing it in a transform node - and setting its 
		     orientation (rotation).

]]
 
-- We create a transform and call it "teapot",
teapot = Transform{
	position = {-1, 2, -3},
	-- set the teapot transform orientation to 90 degrees on the X axis.
	-- Want to rotate on another axis? Just change Axis{0,1,0} for Y or Axis{0,0,1} for Z.
	orientation = AngleAxis(Degrees(90),Axis{1,0,0}),
	Model("assets/models/teapot.osg")
}

-- We attach it relative to the world to add it to our scene
RelativeTo.World:addChild(teapot)

