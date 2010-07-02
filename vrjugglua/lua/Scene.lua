
function Degrees(d)
	return d * 3.141592653589 / 180
end

function Radians(r)
	return r
end

function Axis(coords)
	return osg.Vec3d(coords[1], coords[2], coords[3])
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
	
	if args.scale ~= nil and isnumber(args.scale) then
		t:setScale(args.scale)
	end
	
	if args.children ~= nil then
		for _, v in ipairs(args.children) do
			if v ~= nil then
				t:addChild(v)
			end
		end
	end
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