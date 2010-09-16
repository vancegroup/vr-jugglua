
function Degrees(d)
	return d * 3.141592653589 / 180
end

function Radians(r)
	return r
end

function Axis(coords)
	return osg.Vec3d(coords[1], coords[2], coords[3])
end

Vec3 = Axis

function Vec(x, y, z, w)
	if w ~= nil then
		return osg.Vec4d(x, y, z, w)
	end

	if z ~= nil then
		return osg.Vec3d(x, y, z)
	end

	if y ~= nil then
		return osg.Vec2d(x, y)
	end

	-- OK, so they didn't pass the arguments one-by-one, but rather
	-- as a list

	if #x == 4 then
		return osg.Vec4d(x[1], x[2], x[3], x[4])
	elseif #x == 3 then
		return osg.Vec3d(x[1], x[2], x[3])
	elseif #x == 2 then
		return osg.Vec2d(x[1], x[2])
	end

	-- Hmm, well, they didn't pass as a list either. Bail!
	return nil, "Must call Vec with 2, 3, or 4 elements!"
end

function AngleAxis(angle, axis)
	return osg.Quat(angle, axis)
end

function Transform(args)
	local t = osg.PositionAttitudeTransform()
	if args.position ~= nil then
		if #args.position ~=3 then
			print("Cannot set position: position must have 3 elements!")
		else
			t:setPosition(osg.Vec3d(
				args.position[1],
				args.position[2],
				args.position[3]))
		end
	end

	if args.orientation ~= nil then
		t:setAttitude(args.orientation)
	end

	if args.scale ~= nil then
		t:setScale(osg.Vec3d(args.scale, args.scale, args.scale))
	end

	if args.children ~= nil then
		for _, v in ipairs(args.children) do
			if v ~= nil then
				t:addChild(v)
			end
		end
	end

	return t
end

function Model(filename)
	local m = osgLua.loadObjectFile(filename)
	local c = osgLua.getTypeInfo(m)

	if c and #c.methods > 0 then
		-- Model loaded successfully
		return m
	else
		print("Warning: failed to load model " .. filename)
		return nil
	end
end
