Actions = {
	_frameActions = {};
}

function Actions.addNodeAction(node, func)
	local co = coroutine.create(func)
	coroutine.resume(co, node, 0)
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
	coroutine.resume(co, 0)
	table.insert(Actions._frameActions, co)
end

function Actions.updateFrameActions()
	local keepers = {}
	for _, v in ipairs(Actions._frameActions) do
		--print(coroutine.status(v))
		--- TODO handle errors here
		assert(coroutine.resume(v, Actions.appProxy:getTimeDelta()))
		if coroutine.status(v) ~= 'dead' then
			table.insert(keepers, v)
		end
	end
	Actions._frameActions = keepers
end
