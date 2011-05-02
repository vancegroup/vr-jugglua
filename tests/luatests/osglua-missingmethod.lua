
print("CTEST_FULL_OUTPUT")
val, message = pcall(function()
	osg.PositionAttitudeTransform():getPostition()
end)

print("val = " .. tostring(val))
print("message = " .. tostring(message))
print("Done!")
