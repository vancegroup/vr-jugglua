require("libosgLua") -- linux / windows require

osgLua.loadWrapper("osg")
osgLua.loadWrapper("osgGA")
osgLua.loadWrapper("osgText")
osgLua.loadWrapper("osgProducer")

root = osg.Group()

transform = osg.PositionAttitudeTransform()
root:addChild(transform)
geode = osg.Geode()
shape = osg.ShapeDrawable()
shape:setShape( osg.Sphere(osg.Vec3(0,0,0),15) )
geode:addDrawable(shape)
transform:addChild(geode)



-- load a file...
transform = osg.PositionAttitudeTransform()
root:addChild(transform)
transform:addChild( osgLua.loadObjectFile("cow.osg") )
transform:setScale( osg.Vec3(0.7,0.7,0.7) )

-- set a update Callback
local value = 0
-- creating classes cost to much.. (try reuse them, use clousures)
local v = osg.Vec3()c = osgLua.NodeCallback( 
	function(node, nodeVisitor)
		-- node is a transform in run time
		v:set(math.sin(value)*30,math.cos(value)*30,0)
		node:setPosition(v) 
		value = value +0.001
	end)
transform:setUpdateCallback(c)

text = osgText.Text()
text:setCharacterSize(4)
text:setPosition(osg.Vec3(0,0,18))
text:setAxisAlignment(osgText.Text.AxisAlignment.SCREEN)
text:setAlignment(osgText.Text.AlignmentType.CENTER_BASE_LINE)
text:setText("Hi all from osgLua!")
text:setColor(osg.Vec4(0.9,0.8,0.2,1.0))
geode:addDrawable(text)

viewer = osgProducer.Viewer()
viewer:setSceneData(root)
viewer:setUpViewer(osgProducer.Viewer.ViewerOptions.STANDARD_SETTINGS)

viewer:realize()

while ( not viewer:done()) do
	viewer:update()
	viewer:frame()
	viewer:sync()
end
