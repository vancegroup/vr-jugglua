
print("CTEST_FULL_OUTPUT")
require("help")

function doSubtest(matrixType, vectorType)
	assert((vectorType(5, 5, 5) * matrixType.identity()) == vectorType(5, 5, 5))
	assert((vectorType(5, 5, 5) * matrixType.identity()) == vectorType(5, 5, 5))
end

function doTest(matrixType)
	print(" --- new test --- ")
	print("Matrix type: " .. help.lookup(matrixType())["class"])
	
	print(matrixType.identity())
	
	print(matrixType.identity() == matrixType.identity())
	assert(matrixType.identity() == matrixType.identity())
	
	assert((matrixType.identity() * matrixType.identity()) == matrixType.identity())
	
	doSubtest(matrixType, osg.Vec3)
	doSubtest(matrixType, osg.Vec3f)
	doSubtest(matrixType, osg.Vec3d)
end

doTest(osg.Matrix)
doTest(osg.Matrixd)
doTest(osg.Matrixf)

print("Done!")
