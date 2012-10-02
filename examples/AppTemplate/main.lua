-- Main file for application. It tells VR JuggLua where to look, then runs
-- the code for the app.

-- If you just want the "where to look" part, and want to work on the rest
-- interactively, run appdir.lua instead of this until you have the code
-- the way you like in this file (and possibly others).

-- These lines tell VR JuggLua to look in this directory for models and scripts
require "AddAppDirectory"
AddAppDirectory()

-- This line runs the loadModels script and saves the results in a variable named models
models = runfile "loadModels.lua"

-- This section adds two models loaded by the other script to the world.
RelativeTo.World:addChild(
	Group{
		models.teapot,
		models.ball
	}
)


-- Slightly more advanced: This section adds a FrameAction to spin the teapot.

require "Actions"
Actions.addFrameAction(
	function(dt)
		local angle = 0
		local rate = 30 -- degrees per second

		-- Loop forever
		while true do

			-- dt is the seconds (that is, the fraction of a second) that we waited
			dt = Actions.waitForRedraw()

			-- Update the angle
			angle = angle + rate * dt

			-- Use new angle to update teapot transform
			models.teapot:setAttitude(AngleAxis(Degrees(angle), Axis{0, 1, 0}))
		end
	end
)
