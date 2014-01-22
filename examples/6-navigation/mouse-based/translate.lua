require("AddAppDirectory")
require("StockModels")
AddAppDirectory()

--MUST REMOVE STANDARD NAVIGATION BEFORE ADDING CUSTOM NAVIGATION
osgnav.removeStandardNavigation()

-- add object to scene (to perceive relative navigation)
local teapot = Transform{
	StockModels.Teapot(),
}
RelativeTo.World:addChild(teapot)

-- helper function to translate the world
function translateWorld(x, y, z)
        RelativeTo.World:postMult(osg.Matrixd.translate(x, y, z))
end

-- frame action for basic translation navigation in all directions
Actions.addFrameAction(
	function()
        local translationButton = gadget.DigitalInterface("VJButton0")
        local wand = gadget.PositionInterface('VJWand')
        local rate = 1
        while true do
			repeat
				dt = Actions.waitForRedraw()
			until translationButton.justPressed

			while translationButton.pressed do
				dt = Actions.waitForRedraw()
				local translate_value_x = wand.forwardVector:x() * rate * dt
				local translate_value_y = wand.forwardVector:y() * rate * dt
				local translate_value_z = wand.forwardVector:z() * rate * dt
				translateWorld(-translate_value_x, -translate_value_y, -translate_value_z)
			end
        end
	end
)