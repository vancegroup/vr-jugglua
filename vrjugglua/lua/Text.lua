osgLua.loadWrapper("osgText")

require("help")

local knownFonts = {}
local listOfKnownFonts = {}
function addKnownFont(name, filename)
	if not knownFonts[name] then
		table.insert(listOfKnownFonts, name)
	end
	knownFonts[name] = filename
end

function addKnownFonts(arg)
	for name, filename in pairs(arg) do
		addKnownFont(name, filename)
	end
end

addKnownFonts{
	["DroidSans"] = "assets/fonts/droid-fonts/DroidSans.ttf",
	["DroidSansBold"] = "assets/fonts/droid-fonts/DroidSans-Bold.ttf",
	["DroidSansMono"] = "assets/fonts/droid-sans-mono-slashed/DroidSansMonoSlashed.ttf",
	["DroidSerifBold"] = "assets/fonts/droid-fonts/DroidSerif-Bold.ttf",
	["DroidSerifBoldItalic"] = "assets/fonts/droid-fonts/DroidSerif-BoldItalic.ttf",
	["DroidSerifItalic"] = "assets/fonts/droid-fonts/DroidSerif-Italic.ttf",
	["DroidSerif"] = "assets/fonts/droid-fonts/DroidSerif-Regular.ttf"
}


local fontCache = {}
Font = help.docstring{[[
Pass a filename, or the name of a "known font," to load that font
and return the osg.Font value.
]],
	knownFonts = listOfKnownFonts
} .. function (filename)
	if knownFonts[filename] then
		filename = knownFonts[filename]
	end
	if fontCache[filename] then
		return fontCache[filename]
	end
	local f = osgLua.loadObjectFile(filename)
	local c = osgLua.getTypeInfo(f)
	if not c.name == "osgText::Font" then
		error(string.format("Could not load a font from %s - loaded %s instead!", filename, c.name), 2)
	end
	fontCache[filename] = f
	return f
end




function verifyFont(obj)
	local c = osgLua.getTypeInfo(obj)
	if not c then
		error(string.format("Not even an osgLua type: %s", type(f)), 2)
	end
	if not c.name == "osgText::Font" then
		error(string.format("Not a font: %s instead!", c.name), 2)
	end
end

local defaultFont
function setDefaultFont(f)
	if type(f) == "string" then
		f = Font(f)
	end
	verifyFont(f)
	defaultFont = f
end


TextGeode = help.docstring{[[
Pass a table containing one or more strings, and optionally
some named parameters. It returns an OSG node (Geode) with
the text strings formatted as lines.
]]
optionalParameters = {
	color = "An osg.Vec4 providing RGBA color, with each component in [0,1]: defaults to a light blue",
	lineHeight = "Height of a line/character size: defaults to 0.45",
	position = "Position of the upper-left of the text, in {x, y, z} format",
	font = "A font object, such as that returned from Font()"
}} .. function(arg, ...)
	if type(arg) ~= "table" then
		arg = {arg, ...}
	end
	if arg.font then
		verifyFont(arg.font)
	end
	local color = arg.color or osg.Vec4(0.1,0.1,0.8,1.0)
    local lineHeight = arg.lineHeight or 0.45
    local pos = arg.position or {2.5, -1.5, -7.0}
    local topLineY = pos[2]
    local X = pos[1]
    local Z = pos[3]
    local geode = osg.Geode()
    for i, v in ipairs(arg) do
        local thisLine = osgText.Text()
        thisLine:setFont(arg.font or defaultFont)
        thisLine:setCharacterSize(lineHeight)
        thisLine:setPosition(osg.Vec3(X,topLineY - i * lineHeight,Z))
        thisLine:setText(v)
        thisLine:setColor(color)
        geode:addDrawable(thisLine)
    end
	return geode
end

--[[
local fontCache3D = {}
function Font3D(filename)
	if not osgDB then
		osgLua.loadWrapper("osgDB")
	end

	if knownFonts[filename] then
		filename = knownFonts[filename]
	end
	if fontCache3D[filename] then
		return fontCache3D[filename]
	end
	local rw = osgDB.Registry.instance():getReaderWriterForExtension(".ttf")
	local f = rw:readObject(filename, osgDB.ReaderWriter.Options("3D"))
	local c = osgLua.getTypeInfo(f)
	if not c.name == "osgText::Font3D" then
		error(string.format("Could not load a Font3D from %s - loaded %s instead!", filename, c.name), 2)
	end
	fontCache3D[filename] = f
	return f
end
function Text3DGeode(arg)
	if type(arg) ~= "table" then
		arg = {arg}
	end
	if arg.font then
		verifyFont(arg.font)
	end
	local color = arg.color or osg.Vec4(0.9,0.8,0.7,1.0)
    local lineHeight = arg.lineHeight or 0.45
    local pos = arg.position or {2.5, -1.5, -7.0}
    local topLineY = pos[2]
    local X = pos[1]
    local Z = pos[3]
    local geode = osg.Geode()
    for i, v in ipairs(arg) do
        local thisLine = osgText.Text3D()
        thisLine:setFont(arg.font or defaultFont)
        thisLine:setCharacterSize(lineHeight)
        thisLine:setPosition(osg.Vec3(X,topLineY - i * lineHeight,Z))
        thisLine:setText(v)
        thisLine:setColor(color)
        geode:addDrawable(thisLine)
    end
	return geode
end
]]