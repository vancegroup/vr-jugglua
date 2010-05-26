print("In Lua script initialize-devices.lua")

vrjKernel.loadConfigFile("standalone.jconf")
vrjKernel.start()

wand = gadget.PositionInterface()
wand:init("VJ Wand")


button = gadget.ButtonInterface()
wand:init("VJButton0")

vrjKernel.stop()
