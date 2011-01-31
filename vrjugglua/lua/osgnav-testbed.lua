print("osgnav-testbed.lua starting up...")

require("nav-app-skeleton")

print("Setting up run buffer")
runbuf = vrjSync.RunBuffer(nil)

-- Load config files
require("vrjlua-config")