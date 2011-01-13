
print("CTEST_FULL_OUTPUT")

print(osg.Matrix.identity())

print(osg.Matrix.identity() == osg.Matrix.identity())
assert(osg.Matrix.identity() == osg.Matrix.identity())

assert((osg.Matrix.identity() * osg.Matrix.identity())== osg.Matrix.identity())

print("Done!")
