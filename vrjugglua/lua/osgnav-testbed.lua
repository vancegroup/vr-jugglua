print("osgnav-testbed.lua starting up...")

require("nav-app-skeleton")

print("Setting up run buffer")
runbuf = vrjSync.RunBuffer(nil)

local function usage()
	print(arg[0] .. " [files...] [options...]")
	print([[
Files are handled according to their extension:

	.jconf			Adds config elements from this JConf file.
	      			Disables the "fallback" configuration specified in
	      			'vrjlua-config.lua'. Ignored if started as a
	      			cluster secondary node.

	*.lua			Lua scripts to run once the application starts.


Accepted command line options:
]])
	for option, details in pairs(optionFlags) do
		print(option)
		print(details.help)
		print()
	end
	os.exit(1)
end

local function clusterErrorIf(val)
	if val then
		print("ERROR: Can't be both a primary and secondary node!")
		os.exit(1)
	end
end

local isPrimary = false
local function primary()
	print("Setting up as the cluster primary node...")

	clusterErrorIf(isSecondary)
	vrjKernel.initAsClusterPrimaryNode()
	isPrimary = true
end

local isSecondary = false
local function secondary()
	print("Setting up as a cluster secondary node...")

	clusterErrorIf(isPrimary)
	vrjKernel.initAsClusterSecondaryNode()
	isSecondary = true
end

-- Option flags that we recognize, their help text, and what to do when we see them.
local optionFlags = {
	["--help"] = {
		help = "display this help.";
		action = usage;
	};
	["--cluster-primary"] = {
		help = "Indicates that this computer is in a cluster serving as " ..
		"the primary node.";
		action = primary;
	};
	["--vrjmaster"] = {
		help = "Synonym for --cluster-primary, accepted for compatibility.";
		action = primary;
	};
	["--cluster-secondary"] = {
		help = "This computer is in a cluster serving as a secondary node. " ..
		"It ignores configuration commands, instead receiving its " ..
		"configuration from the primary node.";
		action = secondary;
	};
	["--vrjslave"] = {
		help = "Synonym for --cluster-secondary, accepted for compatibility.";
		action = secondary;
	};
}

local jconfs = {}

local newarg = {}

-- Parse the command line arguments
if not arg or type(arg) ~= "table" then
	arg = {}
end
for _, argument in ipairs(arg) do
	if optionFlags[argument] then
		-- this is an option, not a file, and we recognize it!
		optionFlags[argument].action()

	elseif argument:find("[.]jconf$") then
		-- OK, a jconf to load
		table.insert(jconfs, argument)

	elseif argument:find("[.]lua$") then
		-- OK, a lua file to add to the run buffer
		runbuf:addFile(argument, false)

	else
		-- We don't recognize it - let the user's script handle it.
		table.insert(newarg, argument)
	end
end

-- Update arg
newarg[0] = arg[0]
arg = newarg


if isSecondary then
	print("We're a secondary cluster node - skipping the rest of configuration.")
	return
end

-- Did we get some jconf files?
if #jconfs > 0 then
	-- yes
	print("Loading jconf files specified on command line...")
	for _, jconf in ipairs(jconfs) do
		print("Loading: " .. jconf)
		vrjKernel.loadConfigFile(jconf)
	end
	print("Done loading jconf files specified on command line!")
else
	-- no, loading the fallback
	print("No jconf files specified on command line, loading vrjlua-config.lua to find a fallback")
	require("vrjlua-config")
end
