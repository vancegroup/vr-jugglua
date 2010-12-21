
print("CTEST_FULL_OUTPUT")

group = osg.Group()
visitor = osg.ComputeBoundsVisitor()
print("About to send the visitor in.")
group:accept(visitor)
print("OK, sent the visitor in!")
