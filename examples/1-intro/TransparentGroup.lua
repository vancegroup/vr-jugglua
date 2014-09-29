--[[
EXAMPLE: TransparentGroup.lua
PREREQUISITE: LoadModel.lua, Group.lua, and LoadDirectly.lua
DESCRIPTION: A TransparentGroup is just like a Group (or a Transform) - except that it takes
			 a parameter alpha which is a value between 0 and 1 for opacity.

			 If you're interested in the inner workings, try looking at the
			 TransparentGroup file in the share/vrjugglua/lua directory.
			 It's fairly readable, and you'll see how it literally is a group with some
			 special state applied.
]]

require "TransparentGroup"

RelativeTo.World:addChild(
	TransparentGroup{
		-- set alpha value halfway between invisible and opaque
		alpha = 0.5,

		-- a teapot model
		Model("assets/models/teapot.osg")
	}
)
