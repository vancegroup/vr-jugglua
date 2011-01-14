require("lunatest.lunatest")
require("help")

function test_concat()
	local a = help.docstring[[This is an example]] .. function()
		return "function return value"
	end
	assert_function(a)
	assert_string(a())
end

function test_concat_lookup()
	local a = help.docstring[[This is an example]] .. function()
		return "function return value"
	end
	assert_string(help.lookup(a))
end

function test_concat_twice()
	local a = help.docstring[[This is an example]] .. help.docstring[[This is another example]] .. function()
		return "function return value"
	end
	assert_function(a)
	assert_string(a())
end

function test_concat_twice_lookup()
	local a = help.docstring[[This is an example]] .. help.docstring[[This is another example]] .. function()
		return "function return value"
	end
	assert_table(help.lookup(a))
	assert_len(2, help.lookup(a))
end

function test_call()
	local function a()
		return "function return value"
	end
	help.docstring[[This is an example]](a)
	assert_function(a)
	assert_string(a())
end

function test_call_chain()
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
	local function a()
		return "function return value"
	end
	help.docstring[[This is an example]](a)
	assert_string(help.lookup(a))
end

function test_call_twice()
	local function a()
		return "function return value"
	end
	help.docstring[[This is an example]](a)
	help.docstring[[This is another example]](a)
	assert_function(a)
	assert_string(a())
end

function test_call_twice_lookup()
	local function a()
		return "function return value"
	end
	help.docstring[[This is an example]](a)
	help.docstring[[This is another example]](a)
	assert_table(help.lookup(a))
	assert_len(2, help.lookup(a))
end


function test_applyto()
	local function a()
		return "function return value"
	end
	help.docstring[[This is an example]].applyTo(a)
	assert_function(a)
	assert_string(a())
end

function test_applyto_chain()
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
	local function a()
		return "function return value"
	end
	help.docstring[[This is an example]].applyTo(a)
	assert_string(help.lookup(a))
end

function test_applyto_twice()
	local function a()
		return "function return value"
	end
	help.docstring[[This is an example]].applyTo(a)
	help.docstring[[This is another example]].applyTo(a)
	assert_function(a)
	assert_string(a())
end

function test_applyto_twice_lookup()
	local function a()
		return "function return value"
	end
	help.docstring[[This is an example]].applyTo(a)
	help.docstring[[This is another example]].applyTo(a)
	assert_table(help.lookup(a))
	assert_len(2, help.lookup(a))
end


lunatest.run()