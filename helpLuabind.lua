if not class_info then
  error("Cannot load help Luabind support: must register class_info from the C++ side", 1)
end

local function luabindHelp(obj)
  local h = class_info(obj)
  return { name = h.name,
    methods = h.methods,
    attributes = h.attributes
  }
end

help.addHelpExtension(luabindHelp)
