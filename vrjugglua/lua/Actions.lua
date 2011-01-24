require("help")

Actions = {
	_frameActions = {};
}


Actions.addNodeAction = help.docstring[[
Given a node and a function, creates an action for the node to perform.
By this, we mean: if the node is attached to the scenegraph, this Action
will run.
]] .. function(node, func)
	local co = coroutine.create(func)
	local c = osgLua.NodeCallback(
		function(n, nodeVisitor)
			if coroutine.status(co) == 'dead' then
				node:removeUpdateCallback(c)
			else
				coroutine.resume(co, Actions.appProxy:getTimeDelta())
			end
			--c:traverse(n, nodeVisitor)
		end)
	node:addUpdateCallback(c)
end

Actions.addFrameAction = help.docstring[[
Given a function, start an action. Your function will be run, and
you will be responsible for calling Actions.waitForRedraw() once you
are done executing for a single display frame.

Your action will be removed when it finishes (goes to the end or returns),
or sooner if you use Actions.removeFrameAction.

Returns an object referring to your action - only useful to remove it.
If you don't plan on removing the action before it completes, you don't
need to hang on to this value.
]] .. function (func)
	local co = coroutine.create(func)
	table.insert(Actions._frameActions, co)
	return co
end

Actions.removeFrameAction = help.docstring[[
Remove a frame action, stopping it from executing, presumably prior to
its natural exit.
]] .. function (action)
	local n
	for i,v in ipairs(Actions._frameActions) do
		if v == action then
			n = v
		end
	end
	if n ~= nil then
		table.remove(Actions._frameActions, n)
	end		
end

Actions.waitForRedraw = help.docstring[[
Pauses an action momentarily to update the display. This must be called frequently:
changes will not be seen until it is called.

If you pass a number, it will wait for that many redraws. By default, it
only waits once.

Returns the amount of time that it waited, in seconds.
]] .. function (num)
	if type(num) == "number" then
		local dt = 0
		for i=1,num do
			dt = dt + coroutine.yield()
		end
		return dt
	else
		return coroutine.yield()
	end
end

Actions.updateFrameActions = help.docstring[[
Run all frame actions until they wait for the next redraw. This
should be placed preferably in the latePreFrame section of the application
delegate.

If that made no sense to you, you do not need to use this function.
]] .. function ()
	local thisTime = Actions._frameActions
	Actions._frameActions = {}

	for _, v in ipairs(thisTime) do
		--print(coroutine.status(v))
		--- TODO handle errors here
		assert(coroutine.resume(v, Actions.appProxy:getTimeDelta()))
		if coroutine.status(v) ~= 'dead' then
			table.insert(Actions._frameActions, v)
		end
	end
end

Actions.setAppProxy = help.docstring[[
If you are implementing your own application delegate, call this method
before adding any actions, and pass the value you got from vrjApp.OsgAppProxy().

If this made no sense to you, you do not need to use this function.
]] .. function(proxy)
	Actions.appProxy = proxy
end
