print("Loading lua file")
print("CTEST_FULL_OUTPUT")

testapp = {preFrameCounter = 0,
	latePreFrameCounter = 0,
	intraFrameCounter = 0,
	postFrameCounter = 0,
	max = 10}


function testapp:initScene()
	print("In initScene")
end


function testapp:preFrame(dt)
	self.preFrameCounter = self.preFrameCounter + 1
	print(tostring(self.preFrameCounter) .. ": In preFrame")
end

function testapp:latePreFrame(dt)
	
	self.latePreFrameCounter = self.latePreFrameCounter + 1
	print(tostring(self.latePreFrameCounter) .. ": In latePreFrame")
	if self.latePreFrameCounter ~= self.preFrameCounter then
		print("LUA_TEST_FAIL - counter mismatch!")
		-- Force exit here
		os.sysexit(1)
	end
end

function testapp:intraFrame(dt)
	self.intraFrameCounter = self.intraFrameCounter + 1
	print(tostring(self.intraFrameCounter) .. ": In intraFrame")
	if self.intraFrameCounter ~= self.latePreFrameCounter then
		print("LUA_TEST_FAIL - counter mismatch!")
		-- Force exit here
		os.sysexit(1)
	end
end

function testapp:postFrame(dt)
	self.postFrameCounter = self.postFrameCounter + 1
	print(tostring(self.postFrameCounter) .. ": In postFrame")
	if self.postFrameCounter ~= self.intraFrameCounter then
		print("LUA_TEST_FAIL - counter mismatch!")
		-- Force exit here
		os.sysexit(1)
	end
	if self.postFrameCounter >= self.max then
		print("Counter at maximum")
		vrjKernel.stop()
	end
end


	

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
