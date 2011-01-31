
print("CTEST_FULL_OUTPUT")
local vec = osg.Vec3d(5, 5, 5)
local vec2 = osg.Vec3d(52, 25, 52)
local mat = osg.Matrix.scale(vec)
print(mat.Scale)
assert(vec == mat.Scale)

mat.Trans = vec2
print(mat.Trans)

assert(vec2 == mat.Trans)
assert(vec == mat.Scale)

require("help")
help(mat)

print("Done!")
