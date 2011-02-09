-- Here we are loading a model, and transforming it to a new position
plane = Transform{
	position = {-1, -3, -3},
	Model("examples/models/cessna.osg")
}

-- We attach it relative to the world: we can move around it.
RelativeTo.World:addChild(plane)

-- For the next part, we'd like to put some text
require("Text")

-- Here, we are attaching something relative to the room:
-- it always stays the same place in physical real space.
RelativeTo.Room:addChild(
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