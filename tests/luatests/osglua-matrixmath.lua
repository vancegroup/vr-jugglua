
print("CTEST_FULL_OUTPUT")

print(osg.Matrix.identity())

print(osg.Matrix.identity() == osg.Matrix.identity())
assert(osg.Matrix.identity() == osg.Matrix.identity())

assert((osg.Matrix.identity() * osg.Matrix.identity()) == osg.Matrix.identity())

assert((osg.Vec3f(5, 5, 5) * osg.Matrix.identity()) == osg.Vec3f(5, 5, 5))
assert((osg.Vec3(5, 5, 5) * osg.Matrix.identity()) == osg.Vec3(5, 5, 5))

print("Done!")
