DrawableShapes = {
	Sphere = function(a)
		local pos = osg.Vec3(0.0, 0.0, 0.0)
		if a.position then
			pos:set(unpack(a.position))
		end
		local sphere = osg.Sphere(pos, a.radius or 1.0)
		local drbl = osg.ShapeDrawable(sphere)
		local color = osg.Vec4(0,0,0,0)
		if a.color then
			color:set(unpack(a.color))
		end
		drbl:setColor(color)
		local geode = osg.Geode()
		geode:addDrawable(drbl)
		return geode
	end,
	Cylinder = function (a)
		local pos = osg.Vec3(0.0, 0.0, 0.0)
		if a.position then
			pos:set(unpack(a.position))
		end
		local drbl = osg.ShapeDrawable(osg.Cylinder(pos, a.radius or 1.0, a.height or 1.0))
		local color = osg.Vec4(0,0,0,0)
		if a.color then
			color:set(unpack(a.color))
		end
		drbl:setColor(color)
		local geode = osg.Geode()
		geode:addDrawable(drbl)
		return geode
	end,
	Cube = function(a)
		local pos = osg.Vec3(0.0, 0.0, 0.0)
		if a.position then
			pos:set(unpack(a.position))
		end
		local drbl = osg.ShapeDrawable(osg.Box(pos, a.width or 1.0))
		local color = osg.Vec4(0,0,0,0)
		if a.color then
			color:set(unpack(a.color))
		end
		drbl:setColor(color)
		local geode = osg.Geode()
		geode:addDrawable(drbl)
		return geode
	end,
	Box = function(a)
		local pos = osg.Vec3(0.0, 0.0, 0.0)
		if a.position then
			pos:set(unpack(a.position))
		end
		local drbl = osg.ShapeDrawable(osg.Box(pos, a.width or 1.0,a.height or 1.0,a.depth or 1.0))
		local color = osg.Vec4(0,0,0,0)
		if a.color then
			color:set(unpack(a.color))
		end
		drbl:setColor(color)
		local geode = osg.Geode()
		geode:addDrawable(drbl)
		return geode
	end,
	Capsule = function(a)
		local pos = osg.Vec3(0.0, 0.0, 0.0)
		if a.position then
			pos:set(unpack(a.position))
		end
		local drbl = osg.ShapeDrawable(osg.Capsule(pos, a.radius or 1.0,a.height or 1.0))
		local color = osg.Vec4(0,0,0,0)
		if a.color then
			color:set(unpack(a.color))
		end
		drbl:setColor(color)
		local geode = osg.Geode()
		geode:addDrawable(drbl)
		return geode
	end,
	Cone = function(a)
		local pos = osg.Vec3(0.0, 0.0, 0.0)
		if a.position then
			pos:set(unpack(a.position))
		end
		local drbl = osg.ShapeDrawable(osg.Cone(pos, a.radius or 1.0,a.height or 1.0))
		local color = osg.Vec4(0,0,0,0)
		if a.color then
			color:set(unpack(a.color))
		end
		drbl:setColor(color)
		local geode = osg.Geode()
		geode:addDrawable(drbl)
		return geode
	end,
}