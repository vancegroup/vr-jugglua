project = 'VR JuggLua'
description = 'A Framework for VR Applications Combining Lua, OpenSceneGraph, and VR Juggler'
add_language_extension('cpp','c')
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

	"../../third-party/stdlib/modules",
}

no_summary = true
no_return_or_parms = true
format = 'markdown'
