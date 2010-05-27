print("Loading lua file")

testapp = {
	counter = 0,
	max = 3,

	initScene = function (self)
		print("In initScene - setting devices")
		self.head = gadget.PositionInterface()
		self.head:init("VJHead")

		self.button = gadget.DigitalInterface()
		self.button:init("VJButton0")

		--self.analog = gadget.AnalogInterface()
		--self.analog:init("VJAnalogAxis0")
	end
	,

	preFrame = function (self)
		print("In preFrame")
		self.counter = self.counter + 1
		print("Counter at " .. tostring(self.counter))
		if self.counter >= self.max then
			print("Counter at maximum")
			vrjKernel.stop()
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
vrjKernel.start()

print("Waiting for kernel stop")
vrjKernel.waitForKernelStop()
