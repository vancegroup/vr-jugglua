plane = Transform{
	position = {-1, -3, -3},
	Model("examples/models/cessna.osg")
}
RelativeTo.World:addChild(plane)


require("Text")

RelativeTo.World:addChild(
	TextGeode{
		"First line",
		"second line",
		font = Font("DroidSans"),		
	}
)