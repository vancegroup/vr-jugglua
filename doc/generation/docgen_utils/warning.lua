
local newissueurl = [[https://github.com/vance-group/vr-jugglua/issues/new]]
local getSearchUrl = function(name)
	return [[https://github.com/vance-group/vr-jugglua/issues/search?q=]] .. name
end

local createlink = require "docgen_utils.createlink"

local warnNum = 0
local warn = function(name, msg)
	warnNum = warnNum + 1
	io.stderr:write(("WARNING #%d: entity %q: %s\n"):format(warnNum, name, msg))

	return {"Documentation not created for this entity. If not already filed (see issue-search), please file a new issue.",
		["Search Issues"] = createlink{
			text = ("Issue search for %q"):format(name),
			url = [[https://github.com/vance-group/vr-jugglua/issues/search?q=]] .. name
		},
		["File a New Issue"] = createlink{
			text = "Project New Issue Page",
			url = [[https://github.com/vance-group/vr-jugglua/issues/new]]
		},
	}
end

return warn
