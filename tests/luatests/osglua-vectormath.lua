
print("CTEST_FULL_OUTPUT")

function doTest3(vectype)
	local a = vectype(1, 1, 1)
	local b = vectype(2, 2, 2)
	doTest(a,b)
end

function doTest4(vectype)
	local a = vectype(1, 1, 1, 1)
	local b = vectype(2, 2, 2, 2)
	doTest(a,b)
end

function doTest(a, b)
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

doTest3(osg.Vec3)
doTest3(osg.Vec3d)
doTest3(osg.Vec3f)


doTest4(osg.Vec4)
doTest4(osg.Vec4d)
doTest4(osg.Vec4f)

print("Done!")
