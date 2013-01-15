project = 'VR JuggLua'
description = [[A Framework for VR Applications Combining Lua, OpenSceneGraph, and VR Juggler

* [Project Source and Homepage](https://github.com/vance-group/vr-jugglua)
* [Automated Self-Contained Windows Builds](http://hci.iastate.edu/~rpavlik/downloads/vrjugglua/jenkins/?C=M;O=D)

This file includes documentation for the Lua built-in libraries, as well as
osgLua and the VR JuggLua modules themselves.
]]

file = {
	"osgLua.luadoc",
	"LDoc/ldoc/builtin/global.luadoc",
	"LDoc/ldoc/builtin/coroutine.luadoc",
	"LDoc/ldoc/builtin/debug.luadoc",
	"LDoc/ldoc/builtin/os.luadoc",
	"LDoc/ldoc/builtin/string.luadoc",
	"LDoc/ldoc/builtin/io.luadoc",
	"LDoc/ldoc/builtin/table.luadoc",
	"LDoc/ldoc/builtin/package.luadoc",
	"LDoc/ldoc/builtin/math.luadoc",

	--"../../third-party/stdlib/modules",
}

examples = {
	"../../examples",
	exclude = {
	"../../examples/advanced",
	"../../examples/in-progress",
	}
}
readme = {
	"../ComplexState.md",
	"../Links.md",
	"../../README.mkd"
}
no_summary = true
no_return_or_parms = true
format = 'discount'
