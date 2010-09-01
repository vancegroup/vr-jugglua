-- osg Wrapper enabled from the script
local geode = osg.Geode()
local shape = osg.ShapeDrawable()
shape:setShape( osg.Sphere(osg.Vec3(0,0,0),15) )
geode:addDrawable(shape)
root:addChild(geode)

-- we will now load osgText from the script
osgLua.loadWrapper("osgText")

text = osgText.Text()
text:setCharacterSize(4)
text:setPosition(osg.Vec3(0,0,18))
text:setAxisAlignment(osgText.Text.AxisAlignment.SCREEN)
text:setAlignment(osgText.Text.AlignmentType.CENTER_BASE_LINE)
text:setText("Hi all from osgLua!")
text:setColor(osg.Vec4(0.9,0.8,0.2,1.0))
geode:addDrawable(text)
