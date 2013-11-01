print("CTEST_FULL_OUTPUT")
require "Scene"

function runtest(script)
    print("Testing:", script)
    loadstring(script)()
    print("OK, it didn't crash")
end
runtest [[MatrixTransform{}.Matrix = osg.Matrixd()]]
runtest [[MatrixTransform{}.Matrix = osg.Matrixf()]]
runtest [[MatrixTransform{}.Matrix = MatrixTransform{}.Matrix]]


print("Done!")
