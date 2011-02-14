require("Scene")

StockModels = {}

function StockModels.Axes()
	return Transform{
		scale = 5.0,
		Model("assets/models/osgdata/axes.osg")
	}
end

function StockModels.Teapot()
	return Transform{
		scale = 1.0,
		orientation = AngleAxis(Degrees(-90), Axis{1, 0, 0}),
		Model("assets/models/teapot.osg")
	}
end
