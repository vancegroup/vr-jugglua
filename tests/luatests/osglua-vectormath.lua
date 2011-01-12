
print("CTEST_FULL_OUTPUT")
local a = osg.Vec3(1, 1, 1)
local b = osg.Vec3(2, 2, 2)
local c = a + b
print(c)

local d = b - a
print(d)
print(a == d)

local e = b * 2
print(e)
assert(e == 2 * b)

print("Done!")
