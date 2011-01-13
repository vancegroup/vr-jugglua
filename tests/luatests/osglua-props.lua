
print("CTEST_FULL_OUTPUT")
local vec = osg.Vec3d(5, 5, 5)
local mat = osg.Matrix.scale(vec)
print(mat.Scale)
assert(vec == mat.Scale)
print("Done!")
