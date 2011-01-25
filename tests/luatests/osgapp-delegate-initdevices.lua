print("Loading lua file")
print("CTEST_FULL_OUTPUT")

testapp = {
	counter = 0,
	max = 3,

	initScene = function (self)
		print("In initScene - setting devices")

		print("Setting up position interface")
		self.head = gadget.PositionInterface("VJHead")

		print("Setting up second position interface")
		self.head = gadget.PositionInterface("VJWand")

		print("Setting up Analog interface")
		self.analog = gadget.AnalogInterface("VJAnalogAxis0")

		print("Setting up digital interface")
		self.button = gadget.DigitalInterface("VJButton0")


		-- If we get this far, set up a shutdown preframe
		-- If we fail before here, the proxy will shut down with an error
		self.preFrame = function (self)
			print("In preFrame")
			self.counter = self.counter + 1
			print("Counter at " .. tostring(self.counter))
			if self.counter >= self.max then
				print("Counter at maximum")
				vrjKernel.stop()
			end
		end
	end
}


print("App delegate defined, now creating OsgAppProxy")
testapp.appProxy = vrjApp.OsgAppProxy()

print ("Now setting OsgAppProxy's delegate")
testapp.appProxy:setAppDelegate(testapp)

print("Loading config files into kernel")
vrjKernel.loadConfigFile("standalone.jconf")

print("Setting kernel application")
testapp.appProxy:setActiveApplication()

print("Starting kernel")
vrjKernel.enter()
