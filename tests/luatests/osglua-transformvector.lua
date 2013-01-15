
print("CTEST_FULL_OUTPUT")

function doTest3(vectype)
	local a = vectype(1, 1, 1)
	doTest(a)
end

function doTest4(vectype)
	local a = vectype(1, 1, 1, 1)
	doTest(a)
end

function doTest(a)
	local mat = osg.Matrixf()
	local c = mat * a
	print(c)
end

doTest3(osg.Vec3)
doTest3(osg.Vec3d)
doTest3(osg.Vec3f)


doTest4(osg.Vec4)
doTest4(osg.Vec4d)
doTest4(osg.Vec4f)


print("Done!")
