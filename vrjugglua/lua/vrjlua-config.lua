-- You've probably found this file in someplace/share/vrjugglua/lua/
-- If you copy it to someplace/etc/vrjugglua/ it will take priority over this
-- version, and will also not be overwritten by any new versions.

-- Note that this file is only loaded if there is no jconf file passed on the command line!

print("Loading config files into kernel")
vrjKernel.loadConfigFile("standalone.jconf")

-- Remove this line from your copy in someplace/etc/vrjugglua/ if you're not
-- using a simulator configuration! (It causes the simulator help to be displayed.)
inSimulator = true
