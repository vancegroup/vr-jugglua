--[[
EXAMPLE: DisplayingText.lua
PREREQUISITE: None.
DESCRIPTION: Here we add 2D text to our scene using TextGeode objects.
			 These next examples indicate a variety of things you can do with text.
]]

-- For the next part, we'd like to add some text. The Text module provides a convenient wrapper.
require("Text")

-- a text geode (node) named "title"
title = TextGeode{
	"It's a Title!",
	position = {-1, 3, -8},
	font = Font("DroidSansBold"),
}
-- Now we add "title" to our scene.
RelativeTo.World:addChild(title)

-- a two line text node named "moretext"
moretext = TextGeode{
	"Some body text",
	"and another line!",
	position = {-1, 2, -8},
	font = Font("DroidSans"),
}
-- Now we add "moretext" to our scene.
RelativeTo.World:addChild(moretext)

-- a text node with a more different font named "evenmoretext"
evenmoretext = TextGeode{
	"It's so awesome!",
	position = {-1, 1, -8},
	font = Font("DroidSerifItalic"),
}
-- Now we add "evenmoretext" to our scene
RelativeTo.World:addChild(evenmoretext)
