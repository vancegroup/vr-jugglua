--[[ EXAMPLE: RelativeTo.lua

Builds on DisplayingText.lua so learn that one first.
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

