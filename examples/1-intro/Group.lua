--[[ EXAMPLE: Group.lua

Builds on DisplayingText.lua and AddDirectly.lua
]]

require("Text")

-- A group node can have several children. It doesn't display anything itself in addition.
RelativeTo.World:addChild(
	-- A group node containing
	Group{
		-- a text geode (node)
		TextGeode{
			"It's a Title!",
			position = {-1, 3, -8},
			font = Font("DroidSansBold"),
		},

		-- a two line text node
		TextGeode{
			"Some body text",
			"and another line!",
			position = {-1, 2, -8},
			font = Font("DroidSans"),
		},

		-- a text node with a more different font
		TextGeode{
			"It's so awesome!",
			position = {-1, 1, -8},
			font = Font("DroidSerifItalic"),
		},

	}
)
