require("lunatest.lunatest")

function test_loads()
	require("help")
	assert_table(help)
end

function test_concat()
	require("help")
	local a = help.docstring[[This is an example]] .. function()
		return "function return value"
	end
	assert_function(a)
	assert_string(a())
end

function test_concat_lookup()
	require("help")
	local a = help.docstring[[This is an example]] .. function()
		return "function return value"
	end
	assert_string(help.lookup(a))
end

function test_concat_twice()
	require("help")
	local a = help.docstring[[This is an example]] .. help.docstring[[This is another example]] .. function()
		return "function return value"
	end
	assert_function(a)
	assert_string(a())
end

function test_concat_twice_lookup()
	require("help")
	local a = help.docstring[[This is an example]] .. help.docstring[[This is another example]] .. function()
		return "function return value"
	end
	assert_table(help.lookup(a))
	assert_len(2, help.lookup(a))
end

function test_call()
	require("help")
	local function a()
		return "function return value"
	end
	help.docstring[[This is an example]](a)
	assert_function(a)
	assert_string(a())
end

function test_call_chain()
	require("help")
	local function a()
		return "function return value"
	end
	local function b()
		return 5
	end
	help.docstring[[This is an example]](a)(b)
	assert_function(a)
	assert_string(a())
	assert_string(help.lookup(a))

	assert_function(b)
	assert_number(b())
	assert_string(help.lookup(b))
end

function test_call_lookup()
	require("help")
	local function a()
		return "function return value"
	end
	help.docstring[[This is an example]](a)
	assert_string(help.lookup(a))
end

function test_call_twice()
	require("help")
	local function a()
		return "function return value"
	end
	help.docstring[[This is an example]](a)
	help.docstring[[This is another example]](a)
	assert_function(a)
	assert_string(a())
end

function test_call_twice_lookup()
	require("help")
	local function a()
		return "function return value"
	end
	help.docstring[[This is an example]](a)
	help.docstring[[This is another example]](a)
	assert_table(help.lookup(a))
	assert_len(2, help.lookup(a))
end


function test_applyto()
	require("help")
	local function a()
		return "function return value"
	end
	help.docstring[[This is an example]].applyTo(a)
	assert_function(a)
	assert_string(a())
end

function test_applyto_chain()
	require("help")
	local function a()
		return "function return value"
	end
	local function b()
		return 5
	end
	help.docstring[[This is an example]].applyTo(a).applyTo(b)
	assert_function(a)
	assert_string(a())
	assert_string(help.lookup(a))

	assert_function(b)
	assert_number(b())
	assert_string(help.lookup(b))
end

function test_applyto_lookup()
	require("help")
	local function a()
		return "function return value"
	end
	help.docstring[[This is an example]].applyTo(a)
	assert_string(help.lookup(a))
end

function test_applyto_twice()
	require("help")
	local function a()
		return "function return value"
	end
	help.docstring[[This is an example]].applyTo(a)
	help.docstring[[This is another example]].applyTo(a)
	assert_function(a)
	assert_string(a())
end

function test_applyto_twice_lookup()
	require("help")
	local function a()
		return "function return value"
	end
	help.docstring[[This is an example]].applyTo(a)
	help.docstring[[This is another example]].applyTo(a)
	assert_table(help.lookup(a))
	assert_len(2, help.lookup(a))
end

function test_selfdoc_list_of_functions()
	require("help")
	assert_gt(#help, 1)
	assert_not_nil(help.lookup(help), "The help table has no documentation!")

	local keys = {}
	local foundKeys = {}
	for k,v in pairs(help) do
		table.insert(keys, k)
		foundKeys[k] = false
	end

	for _,v in ipairs(help.lookup(help)["functions"]) do
		assert_not_nil(foundKeys[v], "Function listed in documentation does not exist: " .. v)
		assert_false(foundKeys[v], "Function is listed in documentation more than once: " .. v)
		foundKeys[v] = true
	end

	for k,v in pairs(foundKeys) do
		assert_true(v, "Functions not listed in documentation for help: " .. k)
	end
end

function test_selfdoc_complete()
	require("help")
	for k,v in pairs(help) do
		assert_not_nil(help.lookup(v), "Function help." ..k.. " is not documented")
	end
end

function test_selfdoc_gc()
	require("help")
	collectgarbage("collect")
	assert_gt(#help, 1)
	assert_not_nil(help.lookup(help), "The help table has no documentation following garbage collection!")
end

function test_nodoc_builtins()
	require("help")
	assert_nil(help.lookup(1), "Don't document generic numbers!")
	assert_nil(help.lookup({}), "Don't document generic tables!")
	assert_nil(help.lookup(nil), "Don't document nil!")
	assert_nil(help.lookup("blabla"), "Don't document generic strings!")
	assert_nil(help.lookup(function() end), "Don't document generic functions!")
	assert_nil(help.lookup(coroutine.create(function() end)), "Don't document generic coroutines!")
end

lunatest.run()
