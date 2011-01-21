
function getScriptFilename()
	local info = debug.getinfo(2)
	-- split up the string
	local prefix = string.sub(info.source, 1, 1)
	if prefix == "@" then
		-- this is actually from a file
		return string.sub(info.source, 2)
	else
		return nil, "Not from a file: source is " .. info.source
	end
end
