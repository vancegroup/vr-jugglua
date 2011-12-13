
print("CTEST_FULL_OUTPUT")

geom = osg.Geometry()
require("help")
help(geom)
geom.UseDisplayList = false
assert(not geom.UseDisplayList)
geom.UseDisplayList = true
assert(geom.UseDisplayList)
print("Done!")
