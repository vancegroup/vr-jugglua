require("gldef")
require("StockModels")

--create a local transform as example geometry
local teapot = Transform{
	StockModels.Teapot(),
}

--create a clipplane osg object
local clipplane = osg.ClipPlane()
--set direction of plane clipping (-z)
clipplane:setClipPlane(0.0,0.0,-1.0,0.0)
--set plane number (useful for multiple planes)
clipplane:setClipPlaneNum(0)

--create osg clipnode object (all geometry attached here will be clipped)
clipNode = osg.ClipNode()
--add the clipplane to the clipNode
clipNode:addClipPlane(clipplane)
-- add teapot xform to clipNode
clipNode:addChild(teapot)

--alter stateset
dstate = clipNode:getOrCreateStateSet()
dstate:setRenderBinDetails(4,"RenderBin")
dstate:setMode(gldef.GL_CULL_FACE,osg.StateAttribute.Values.OFF+osg.StateAttribute.Values.OVERRIDE)

--add clipNode to world
RelativeTo.World:addChild(clipNode);


