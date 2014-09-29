--[[
EXAMPLE: Change_Position_Interactively.lua
PREREQUISITE: LoadModel.lua and Transformation.lua
DESCRIPTION: You can enter commands into the VR JuggLua Scenegraph Navigation Testbed Window
			 interactively.
			 This means you do not have to re-run the script everytime you make a change.
			 If you enter a change into the Testbed Window you like, make sure to include
			 it in your script so the changes are saved.
]]

-- 1) Create a transform called "xform" and set its position to (x=0,y=0,z=0)
mySphere = Transform{
	position = {0, 0, 0},
	Sphere{ radius = .5}
}

-- 2) Add mySphere to scene
RelativeTo.World:addChild(mySphere)

-- 3) Run this script in VRJuggLua.

--[[
   4) Once the script is running, move the mySphere transform with the "setPosition" method.
      Enter the following command into the VR JuggLua Scenegraph Navigation Testbed Window
      and hit "Run"
      mySphere:setPosition(Vec(1, 0, 0))
]]

