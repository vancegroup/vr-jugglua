print("In Lua script stop-kernel.lua")

print("Starting kernel...")
vrjKernel.start()

-- Sleep for two seconds to not have a race condition shutting down the kernel
require("sleep")
sleep(2.0)

print("Stopping kernel...")
vrjKernel.stop()
