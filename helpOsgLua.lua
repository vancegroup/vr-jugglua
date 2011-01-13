if not osgLua then
  error("Cannot load help osgLua support: osgLua not found.", 1)
end

local function osgLuaHelp(obj)

end

help.addHelpExtension(osgLuaHelp)
