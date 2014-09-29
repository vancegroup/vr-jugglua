--[[
EXAMPLE: RelativeTo.lua
PREREQUISITE: DisplayingText.lua and AddDirectly.lua
DESCRIPTION: Here we create 2 TextGeodes. We attach one relative to the world
			 and one relative to the room. The one attached relative to the room
			 will always be in the same physical space.
]]

require("Text")

-- We attach it relative to the world: we can move around it.
RelativeTo.World:addChild(
	TextGeode{
		"This text was added to RelativeTo.World",
		position = {-1, 2, -8},
		font = Font("DroidSerifItalic"),
	}
)

-- Here, we are attaching something relative to the room:
-- it always stays the same place in physical real space.
RelativeTo.Room:addChild(
	TextGeode{
		"This text was added to RelativeTo.Room",
		position = {-1, 3, -8},
		font = Font("DroidSansBold"),
	}
)

