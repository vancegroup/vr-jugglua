group = osg.Group()

bbox = getBoundingBox(group)

for k,v in pairs(bbox) do
	print(tostring(k) .. ": " .. tostring(v))
end
