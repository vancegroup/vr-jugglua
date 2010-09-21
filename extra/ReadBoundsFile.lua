

local function order(val1, val2)
	if val1 < val2 then
		return val1, val2
	else
		return val2, val1
	end
end

function ReadBoundsFile(filename)
	local f = io.open(filename)
	local t = f:read("*all")
	return ReadBoundsData(t)
end

function ReadBoundsData(data)
	local raw = loadstring("return " .. data)()
	
	local ret = {}
	
	local i
	for i=1, #raw, 2 do
		local bbox = {}
		bbox.xMin, bbox.xMax = order(raw[i][1], raw[i+1][1])
		bbox.yMin, bbox.yMax = order(raw[i][2], raw[i+1][2])
		bbox.zMin, bbox.zMax = order(raw[i][3], raw[i+1][3])
		table.insert(ret, bbox)
	end
	--[[
	for i, v in ipairs(ret) do
		print(string.format("%d: x (%04f, %04f)  y (%04f, %04f)  z (%04f, %04f)",
			i,
			v.xMin,
			v.xMax,
			v.yMin,
			v.yMax,
			v.zMin,
			v.zMax))
		
	end]]

end

--ReadBoundsFile("samplebounds")
--[[
local f = io.open("FINAL_RESULT.csv")
local t = f:read("*all")
ReadBoundsData("{" .. t .. "}")]]
