
print("CTEST_FULL_OUTPUT")
function doTest(vectype)
	local a = vectype(1, 1, 1)
	local b = vectype(2, 2, 2)
	local c = a + b
	print(c)
	
	local d = b - a
	print(d)
	assert(a == d)
	
	local e = b * 2
	print(e)
	assert(e == 2 * b)
	
	print(-a)
end

doTest(osg.Vec3)

doTest(osg.Vec3d)
doTest(osg.Vec3f)

print("Done!")
