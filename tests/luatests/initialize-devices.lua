print("In Lua script initialize-devices.lua")

vrjKernel.loadConfigFile("standalone.jconf")
vrjKernel.start()

print("Setting up position interface")
wand = gadget.PositionInterface("VJWand")

print("Setting up digital interface")
button = gadget.DigitalInterface("VJButton0")

print("Setting up analog interface")
analog = gadget.AnalogInterface("VJAnalogAxis0")

-- Sleep for two seconds to not have a race condition shutting down the kernel
require("sleep")
sleep(2.0)


print("Stopping kernel...")
vrjKernel.stop()
