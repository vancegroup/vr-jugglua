-- This script will return a table containing models arranged as desired.

return {
	teapot = Transform{
		position = {-1, 2, -3},
		Model("teapot.osg")
	},
	ball = Sphere{
		radius = .5,
		position = {1, 0, 0}
	}
}
