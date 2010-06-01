
local appDelegate = {}
appDelegate.appProxy =  vrjApp.OsgAppProxy()
appDelegate.scripts = vrjSync.RunBuffer(appDelegate)
appDelegate.appProxy:setAppDelegate(appDelegate)

function appDelegate:initScene()
	self.state:enter()
end

function appDelegate:preFrame()
	local dt = self.appProxy:getTimeDelta()
	self.state:update(dt)
	if (self.state.events ~= nil) then
		--print("About to loop through the event handlers")
		for event, action in pairs(self.state.events) do
			--print(tostring(event))
			local result = event()
			print ("Result: " .. tostring(result))
			if result == true then
				print("Event " .. tostring(event) .. " occurred, running handler.")
				action(dt)
			else	
				--print("Event " .. tostring(event) .. " did not occur.") 
			end
		end
	end
end

function appDelegate:postframe()
	self.scripts:runBuffer()
end

local function createStateTransition(state)
	return function()
			moveDirectlyToState(state)
		end
end

local function moveDirectlyToState(state)
	-- Run the leave callback, if it exists
	if (appDelegate.state ~= nil) and (appDelegate.state.leave ~= nil) then
		appDelegate.state:leave()
	end
	
	-- Actually change the state
	appDelegate.state = state
	
	-- Run the enter callback, if it exists
	if (appDelegate.state ~= nil) and (appDelegate.state.enter ~= nil) then
		appDelegate.state:enter()
	end
	
	-- A transition to "nil" state is a signal to stop.
	if state == nil then
		vrjKernel.stop()
	end
end

local function getScene()
	return appDelegate.appProxy:getScene()
end

local function defineStartingState(state)
	appDelegate.state = state
end

local function runApp()
	print("Setting kernel application")
	appDelegate.appProxy:setActiveApplication()

	print("Starting kernel")
	vrjKernel.start()
end

StateMachine = {
	defineStartingState = defineStartingState,
	moveDirectlyToState = moveDirectlyToState,
	createStateTransition = createStateTransition,
	getScene = getScene,
	loadConfigFile = vrjKernel.loadConfigFile,
	waitForStop = vrjKernel.waitForKernelStop,
	runApp = runApp}

return StateMachine