require("getScriptFilename")
fn = getScriptFilename()
assert(fn, "Have to load this from file, not copy and paste, or we can't find our models!")
vrjLua.appendToModelSearchPath(fn)

require("StockModels")

newroom = Transform{
	orientation = AngleAxis(Degrees(-90), Axis{1.0, 0.0, 0.0}),
	scale = ScaleFrom.inches,
	Model("basicfactory.ive")
}


teapot = Transform{
	position = {0, 1, 0},
	StockModels.Teapot()
}
RelativeTo.World:addChild(newroom)
RelativeTo.World:addChild(teapot)

ss = RelativeTo.World:getOrCreateStateSet()

function doLight1()
	--[[
	l1 = Light{
		number = 0,
		ambient = 0.9,
		diffuse = 0.7,
		specular = 0.5,
		position = {0, 1, 1},
		positional = true
	}]]
	--l1:setSpotCutoff(20)
	--l1:setSpotExponent(50)
	l1 = osg.Light()
	ls1 = osg.LightSource()
	ls1:setLight(l1)
	ls1:setLocalStateSetModes(osg.StateAttribute.Values.ON)

	-- This next line is equivalent to
	-- ls1:setStateSetModes(ss, osg.StateAttribute.Values.ON)
	ss:setAssociatedModes(l1, osg.StateAttribute.Values.ON)

	RelativeTo.Room:addChild(
		ls1
	)
	-- some kind of bug in scene.lua that makes it set position of lights wrong
	l1:setPosition(osg.Vec4(0, 1, 1, 1))
end

function doLight2()
	--[[
	l2 = Light{
		number = 1,
		ambient = osg.Vec4(.5, .5, 0.0, .5),
		diffuse = 0.2,
		specular = 0.5,
		direction = {0, -1, 0},
		position = {1.5, 2, 2},
		directional = true -- opposite of positional
	}]]
	l2 = osg.Light()
	l2:setLightNum(1)
	l2:setAmbient(osg.Vec4(.5, .5, 0.0, .5))
	l2:setPosition(osg.Vec4(1.5, 2, 2, 0))
	ls2 = osg.LightSource()
	ls2:setLight(l2)
	ls2:setLocalStateSetModes(osg.StateAttribute.Values.ON)

	GL_LIGHT1 = 0x4001
	--ss:setMode(GL_LIGHT1, 1)
	--ls2:setStateSetModes(ss, osg.StateAttribute.Values.ON)
	ss:setAssociatedModes(l2, osg.StateAttribute.Values.ON)

	RelativeTo.Room:addChild(
		ls2
	)
end