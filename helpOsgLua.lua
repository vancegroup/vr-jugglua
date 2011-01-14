if not osgLua then
  error("Cannot load help osgLua support: osgLua not found.", 1)
end

local function osgLuaHelp(obj)
	local c = osgLua.getTypeInfo(obj)
	if c then
     	local ret = {class = c.name}
		if #(c.constructors) > 0 then
      		ret.constructors = {}
        		for _,v in ipairs(c.constructors) do
        	  	 table.insert(ret.constructors,v)
        		end
        	end
        	if #(c.methods) > 0 then
      		ret.methods = {}
        		for _,v in ipairs(ret.methods) do
        	  	 table.insert(ret.methods,v)
        		end
        	end
        	return ret
	else
		return nil
	end
end

help.addHelpExtension(osgLuaHelp)
