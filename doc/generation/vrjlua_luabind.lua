local document_vrjlua_luabind

local warn
do
	local warnNum = 0
	warn = function(name, msg)
		warnNum = warnNum + 1
		io.stderr:write(("WARNING #%d: entity %q: %s\n"):format(warnNum, name, msg))
	end
end

local construct_args = {
	["gadget.DigitalInterface"] = function() return "VJButton0" end,
}

local typehandlers

typehandlers = {
	["table"] = function(name, val)
		for k, v in pairs(val) do
			if type(k) == "string" then
				document_vrjlua_luabind(("%s.%s"):format(name, k), v)
			else
				document_vrjlua_luabind(("%s[%q]"):format(name, k), v)
			end
		end
	end;

	["luabind_class"] = function(name, val)
		if construct_args[name] == nil then
			return warn(name, "No constructor arguments provided!")
		else
			return typehandlers["luabind_instance"](name, val(construct_args[name]()))
		end
	end;

	["luabind_instance"] = function(name, val)
		local ci = class_info(val)
		print(("Luabind thinks %q is of type %q"):format(name, ci.name))
		for _, method in ipairs(ci.methods) do
			print("Method:", method)
		end
		for _, attribute in ipairs(ci.attributes) do
			print("Attribute:", attribute)
		end
	end;

	["userdata"] = function(name, val)
		if class_info(val).name == "userdata" then
			return typehandlers["luabind_class"](name, val)
		else
			return typehandlers["luabind_instance"](name, val)
		end
	end;
}

document_vrjlua_luabind = function(name, val)
	print(("\nVRJLUA: Documenting %q"):format(name), type(val))
	typehandlers[type(val)](name, val)
end

return {
	["document_vrjlua_luabind"] = document_vrjlua_luabind
}
