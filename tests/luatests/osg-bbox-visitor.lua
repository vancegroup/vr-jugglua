group = osg.Group()
print("CTEST_FULL_OUTPUT")

bbox = getBoundingBox(group)

for k,v in pairs(bbox) do
	print(tostring(k) .. ": " .. tostring(v))
end
