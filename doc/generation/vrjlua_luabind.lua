local document_vrjlua_luabind

local warn = require "generation_warning"

local construct_args = require "vrjlua_inputs.constructor_args"

local typehandlers

local document_entity = function(name, val, determinedtype)
	local t = determinedtype
	if not t then
		print(("\nVRJLUA: Documenting %q"):format(name), type(val))
		t = type(val)
	end
	return typehandlers[t](name, val)
end

typehandlers = {
	["table"] = function(name, val)
		for k, v in pairs(val) do
			if type(k) == "string" then
				document_entity(("%s.%s"):format(name, k), v)
			else
				document_entity(("%s[%q]"):format(name, k), v)
			end
		end
	end;

	["luabind_class"] = function(name, val)
		if construct_args[name] == nil then
			return warn(name, "No constructor arguments provided!")
		else
			local instance = val(construct_args[name]())
			return document_entity(name, instance, "luabind_instance")
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
			return document_entity(name, val, "luabind_class")
		else
			return document_entity(name, val, "luabind_instance")
		end
	end;
}

return {
	["document_entity"] = document_entity
}
