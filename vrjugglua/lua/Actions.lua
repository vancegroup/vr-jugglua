Actions = {
	_frameActions = {};
}

function Actions.addNodeAction(node, func)
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

function Actions.addFrameAction(func)
	local co = coroutine.create(func)
	table.insert(Actions._frameActions, co)
	return co
end

function Actions.updateFrameActions()
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
