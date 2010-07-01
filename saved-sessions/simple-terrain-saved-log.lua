-- navtransform is your root-level group node

-- Simulator Mode Instructions for Navigation --
-- In the simulator window:
--    Hold 'shift' and move the mouse to rotate the simulated wand
--    Click to move in the direction of the wand's tip

baler = osgLua.loadObjectFile("/home/users/cnoon15/source/vrac/vrjAndD3D/models")
navtransform:addChild(baler)

baler = osgLua.loadObjectFile("/home/users/cnoon15/source/vrac/vrjAndD3D/models/baler.3ds")
navtransform:addChild(baler)

terrain = osgLua.loadObjectFile("/home/users/cnoon15/source/vrac/vrjAndD3D/models/terrain_simple.ive")
navtransform:addChild(terrain)

navtransform:removeChild(terrain)
navtransform:removeChild(baler)

rotate = osg.PositionAttitudeTransform()
rotate:setAttitude(osg.Quat(3.1415/2, osg.Vec3d(0, 1, 0)))
rotate:addChild(terrain)
navtransform:addChild(rotate)

rotate:setAttitude(osg.Quat(3.1415/2, osg.Vec3d(1.0, 0, 0)))

rotate:setAttitude(osg.Quat(3.1415/2, osg.Vec3d(-1, 0, 0)))

rotate:setAttitude(osg.Quat(3.1415/2, osg.Vec3d(1.0, 0, 0)))

