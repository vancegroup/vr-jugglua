
print("CTEST_FULL_OUTPUT")

require("Actions")

a = Actions.addFrameAction(function() while true do Actions.waitForRedraw() end end)

Actions.removeFrameAction(a)

