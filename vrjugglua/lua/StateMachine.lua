
local appDelegate = {}

function appDelegate:initScene()
	if (self.init ~= nil) then self.init() end
	self.state:enter()
end

function appDelegate:preFrame()
	local dt = self.appProxy:getTimeDelta()
	self.state:update(dt)
	if (self.state.events ~= nil) then
		for event, action in pairs(self.state.events) do
			if event() then
				print("Event " .. tostring(event) .. " occurred, running handler.")
				action(dt)
			end
		end
	end
end

function appDelegate:latePreFrame()
	self.scripts:runBuffer()
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

local function createStateTransition(state)
	return function()
		moveDirectlyToState(state)
	end
end

local function getScene()
	return appDelegate.appProxy:getScene()
end

local function getNodeTrackingPositionOnly(device)
	appDelegate.appProxy:getNodeTrackingPositionOnly(device)
end

local function getNodeTrackingPose(device)
	appDelegate.appProxy:getNodeTrackingPose(device)
end

local function setInitFunction(f)
	appDelegate.init = f
end

local function setStartingState(state)
	appDelegate.state = state
end

local function runApp()
	print("Setting kernel application")
	appDelegate.appProxy:setActiveApplication()

	print("Starting kernel")
	vrjKernel.enter()
end

StateMachine = {
	setInitFunction = setInitFunction,
	setStartingState = setStartingState,
	moveDirectlyToState = moveDirectlyToState,
	createStateTransition = createStateTransition,
	getScene = getScene,
	getNodeTrackingPositionOnly = getNodeTrackingPositionOnly,
	getNodeTrackingPose = getNodeTrackingPose,
	runApp = runApp}


appDelegate.appProxy = vrjApp.OsgAppProxy()
appDelegate.scripts = vrjSync.RunBuffer()
appDelegate.appProxy:setAppDelegate(appDelegate)

return StateMachine
