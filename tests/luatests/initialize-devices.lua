print("In Lua script initialize-devices.lua")

vrjKernel.loadConfigFile("standalone.jconf")
vrjKernel.start()

wand = gadget.PositionInterface("VJWand")


button = gadget.DigitalInterface("VJButton0")

vrjKernel.stop()
