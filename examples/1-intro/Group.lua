--[[
EXAMPLE: Group.lua
PREREQUISITE: DisplayingText.lua
DESCRIPTION: Here we add a Group node. The group node can contain multiple children.
			 It doesn't display anything itself in addition.
]]
require("Text")

-- A group node containing textgeodes named "myGroup"
myGroup = Group{
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
-- add myGroup to the scene
RelativeTo.World:addChild(myGroup)
