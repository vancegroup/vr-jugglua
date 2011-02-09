
require("Text")
require("Actions")

function makeCountdownNumber(num)
	return TextGeode{
		tostring(num),
		position = {-1, 2, -8},
		font = Font("DroidSerifItalic"),
		lineHeight = 1
	}
end

Actions.addFrameAction(function()
			
	local disappearing = TextGeode{
		"This text will self-destruct in...",
		position = {-1, 3, -8},
		font = Font("DroidSansBold"),		
	}
	RelativeTo.World:addChild(disappearing)
	for i=5,1,-1 do
		local num = makeCountdownNumber(i)
		disappearing:addChild(num)
		Actions.waitSeconds(1)
		disappearing:removeChild(num)
	end
	
	RelativeTo.World:removeChild(disappearing)

end)