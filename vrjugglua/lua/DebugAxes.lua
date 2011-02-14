
require("Scene")

local axes = Transform{
	scale = 5.0,
	Model("assets/models/osgdata/axes.osg")
}

local switch = osg.Switch()
switch:addChild(axes)
switch:setAllChildrenOff()

DebugAxes = {
	node = switch
}

function DebugAxes.show()
	switch:setAllChildrenOn()
end

function DebugAxes.hide()
	switch:setAllChildrenOff()
end

