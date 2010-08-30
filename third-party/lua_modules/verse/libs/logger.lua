local print = print
local select, tostring = select, tostring;
module "logger"

local function format(format, ...)
	local n, maxn = 0, #arg;
	return (format:gsub("%%(.)", function (c) if c ~= "%" and n <= maxn then n = n + 1; return tostring(arg[n]); end end));
end

local function format(format, ...)
	local n, maxn = 0, select('#', ...);
	local arg = { ... };
	return (format:gsub("%%(.)", function (c) if n <= maxn then n = n + 1; return tostring(arg[n]); end end));
end

function init(name)
	return function (level, message, ...)
		print(level, format(message, ...));
	end
end

return _M;
