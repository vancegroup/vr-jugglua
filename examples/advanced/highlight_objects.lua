require("osgFX")

local GraphicsNode = osgFX.Scribe()
--This will "highlight" objects for you, this is useful
--when you want to display to the user that an object
--is selected for example.  It uses the wireframe
--of the object as a visual indication.
GraphicsNode:setWireframeLineWidth(20)

obj = Sphere{ radius = 0.5, position = {0, 0, 0}}

GraphicsNode:addChild(obj)

local GraphicsSwitch = osg.Switch()

GraphicsSwitch:addChild(GraphicsNode)

RelativeTo.World:addChild(GraphicsSwitch)

