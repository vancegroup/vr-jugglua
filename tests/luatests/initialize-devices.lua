print("In Lua script initialize-devices.lua")
print("CTEST_FULL_OUTPUT")

vrjKernel.loadConfigFile("standalone.jconf")
vrjKernel.start()

print("Setting up position interface")
wand = gadget.PositionInterface("VJWand")

print("Setting up digital interface")
button = gadget.DigitalInterface("VJButton0")

print("Setting up analog interface")
analog = gadget.AnalogInterface("VJAnalogAxis0")

print("Stopping kernel...")
vrjKernel.stop()
