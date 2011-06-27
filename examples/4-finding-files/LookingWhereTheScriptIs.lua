

-- If you run a script from a file, rather than copying and pasting, it knows where it is.
-- We can use that to find models and other files in the same directory.

require("getScriptFilename")
fn = getScriptFilename()

print("This is " .. fn)

-- This line will error out if this script wasn't loaded from a file.
assert(fn, "Have to load this from file, not copy and paste, or we can't find our models!")

-- This will add the directory passed (or the parent directory of a file, such as fn)
-- to the model search path.
vrjLua.appendToModelSearchPath(fn)

-- Now we can find the full path of "example.txt"
print(vrjLua.findInModelSearchPath("example.txt"))
