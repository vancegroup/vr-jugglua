require("getScriptFilename")
vrjLua.appendToModelSearchPath(getScriptFilename())

--load in an example model (so we can see the effect of our lights)
factory = Transform{
	orientation = AngleAxis(Degrees(-90), Axis{1.0, 0.0, 0.0}),
	scale = ScaleFrom.inches,
	Model("../models/basicfactory.ive")
}
--add example model 'factory' to the scene graph
RelativeTo.World:addChild(factory)


-- need thw RelativeTo.World's state set to turn on our lights int the scene
worldStateSet = RelativeTo.World:getOrCreateStateSet()

function createLight1()
	-- create a light object
	l1 = osg.Light()
	-- set light number (openGL and osg can have up to eight lights in  a scene #0-7)
	l1:setLightNum(0)
	--  create a lightsource object
	ls1 = osg.LightSource()
	-- turn the light source on
	ls1:setLocalStateSetModes(osg.StateAttribute.Values.ON)
	-- set light l1 to light source ls1
	ls1:setLight(l1)
	
	--set light attributes
	l1:setAmbient(osg.Vec4(0.8, 0.8, 0.8, 0.5))
	l1:setDiffuse(osg.Vec4(0.8, 0.8, 0.8, 0.8))
	l1:setSpecular(osg.Vec4(0.8, 0.8, 0.8, 0.8))
		
	-- turn the light on in the relativeto.world state set
	worldStateSet:setAssociatedModes(l1, osg.StateAttribute.Values.ON)
	--add lightsource to scene (room)
	RelativeTo.Room:addChild(ls1)
	-- set position of light
	l1:setPosition(osg.Vec4(0, 3, -5, 1.0))
end

function createLight2()
	-- create a light object
	l2 = osg.Light()
	-- set light number (openGL and osg can have up to eight lights in  a scene #0-7)
	l2:setLightNum(1)
	--  create a lightsource object
	ls2 = osg.LightSource()
	-- turn the light source on
	ls2:setLocalStateSetModes(osg.StateAttribute.Values.ON)
	-- set light l2 to lightsource ls2
	ls2:setLight(l2)
	--set light attributes
	l2:setAmbient(osg.Vec4(.8, .8, 0.6, .50))
	
	-- turn the light on in the relativeto.world state set
	worldStateSet:setAssociatedModes(l2, osg.StateAttribute.Values.ON)
	
	--add lightsource to scene (room)
	RelativeTo.Room:addChild(ls2)
	-- set position of light	
	l2:setPosition(osg.Vec4(1.5, 2, 2, 1.0))
end

--calls to turn create lights functions (by extention creates them and turns them ON)
createLight1()
createLight2()

