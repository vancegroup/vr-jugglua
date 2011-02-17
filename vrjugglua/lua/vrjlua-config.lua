-- You've probably found this file in someplace/share/vrjugglua/lua/
-- If you copy it to someplace/etc/vrjugglua/ it will take priority over this
-- version, and will also not be overwritten by any new versions.

print("Loading config files into kernel")
vrjKernel.loadConfigFile("standalone.jconf")
