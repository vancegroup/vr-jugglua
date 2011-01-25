print("CTEST_FULL_OUTPUT")

require("Actions")

maxFrames = 10

Actions.addFrameAction(function()
	for i=1,maxFrames do
		print("i = " .. tostring(i))
		Actions.waitForRedraw()
	end
	print("Counter at maximum, adding to runbuffer for exiting next frame")
	runbuf:addString([[
		print("Runbuffer string executing - shutting down successfully.")
		vrjKernel.stop()
		os.exit(0)
	]], false)
end)

print("Loading osgnav-testbed module")
require("osgnav-testbed")

print("Starting kernel")
vrjKernel.enter()
