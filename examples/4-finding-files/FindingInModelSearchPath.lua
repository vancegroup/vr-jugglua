-- You can use the model search path to find models manually, or find other files.

-- This function looks thruogh the model search path for the given file, and if it finds it, it returns the full path.
print(vrjLua.findInModelSearchPath("assets/fonts/droid-fonts/DroidSans.ttf"))

-- In this case, we're looking for a ttf (font) file that is bundled, but because you can modify the search path, you can find all kinds of things.
