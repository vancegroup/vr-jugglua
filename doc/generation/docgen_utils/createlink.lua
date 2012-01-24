local LinkPrototype = {}
local LinkMT = {__index = LinkPrototype}

function LinkPrototype:toMarkdown()
	if self.text == nil then self.text = self.url end
	return ([==[[%s](%s)]]==]):format(self.text, self.url)
end

return function(arg)
	setmetatable(arg, LinkMT)
	return arg
end
