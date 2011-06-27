print("CTEST_FULL_OUTPUT")

require("Actions")
Actions.addFrameAction(function()
	for i=1,5 do
		Actions.waitForRedraw()
	end
	print("OK, shutting down after 5 frames.")
	vrjKernel.stop()
end)
