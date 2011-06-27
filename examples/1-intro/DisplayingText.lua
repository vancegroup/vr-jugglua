-- For the next part, we'd like to put some text. The Text module provides a convenient wrapper.
require("Text")

-- These next examples indicate a variety of things you can do with text.

-- a text geode (node)
title = TextGeode{
	"It's a Title!",
	position = {-1, 3, -8},
	font = Font("DroidSansBold"),
}
RelativeTo.World:addChild(title)

-- a two line text node
moretext = TextGeode{
	"Some body text",
	"and another line!",
	position = {-1, 2, -8},
	font = Font("DroidSans"),
}
RelativeTo.World:addChild(moretext)

-- a text node with a more different font
evenmoretext = TextGeode{
	"It's so awesome!",
	position = {-1, 1, -8},
	font = Font("DroidSerifItalic"),
}
RelativeTo.World:addChild(evenmoretext)
