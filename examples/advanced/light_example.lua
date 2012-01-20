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
	light1 = osg.Light()
	-- set light number (openGL and osg can have up to eight lights in  a scene #0-7)
	light1:setLightNum(0)
	--  create a lightsource object
	lightsource1 = osg.LightSource()
	-- turn the light source on
	lightsource1:setLocalStateSetModes(osg.StateAttribute.Values.ON)
	-- set light l1 to light source ls1
	lightsource1:setLight(light1)
	
	--set light attributes
	light1:setAmbient(osg.Vec4(0.8, 0.8, 0.8, 0.5))
	light1:setDiffuse(osg.Vec4(0.8, 0.8, 0.8, 0.8))
	light1:setSpecular(osg.Vec4(0.8, 0.8, 0.8, 0.8))
		
	-- turn the light on in the relativeto.world state set
	worldStateSet:setAssociatedModes(light1, osg.StateAttribute.Values.ON)
	--add lightsource to scene (room)
	RelativeTo.Room:addChild(lightsource1)
	-- set position of light
	light1:setPosition(osg.Vec4(0, 3, -5, 1.0))
end

function createLight2()
	-- create a light object
	light2 = osg.Light()
	-- set light number (openGL and osg can have up to eight lights in  a scene #0-7)
	light2:setLightNum(1)
	--  create a lightsource object
	lightsource2 = osg.LightSource()
	-- turn the light source on
	lightsource2:setLocalStateSetModes(osg.StateAttribute.Values.ON)
	-- set light light2 to lightsource lightsource2
	lightsource2:setLight(light2)
	--set light attributes
	light2:setAmbient(osg.Vec4(.8, .8, 0.6, .50))
	
	-- turn the light on in the relativeto.world state set
	worldStateSet:setAssociatedModes(light2, osg.StateAttribute.Values.ON)
	
	--add lightsource to scene (room)
	RelativeTo.Room:addChild(lightsource2)
	-- set position of light	
	light2:setPosition(osg.Vec4(1.5, 2, 2, 1.0))
end

--calls to turn create lights functions (by extension creates them and turns them ON)
createLight1()
createLight2()

