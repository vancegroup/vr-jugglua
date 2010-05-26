print("Loading lua file")

function initScene()
	print("in initScene()")
	transform = osg.PositionAttitudeTransform()
	geode = osg.Geode()
	shape = osg.ShapeDrawable()
	shape:setShape( osg.Sphere(osg.Vec3(0,0,0),15) )
	geode:addDrawable(shape)
	transform:addChild(geode)

	return true
end

initScene()