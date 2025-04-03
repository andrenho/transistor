require "ui.scene"
require "ui.context"
require "components.componentdef"
require "components.component"

local bit = require("bit")
local bnot = bit.bnot

local button: ComponentDef = {
   key = "__button",
   type = "single_tile",
   can_rotate = false,
   data_size = 1,
   
   pins = {
      { name = "O1", direction = "output" },
      { name = "O2", direction = "output" },
      { name = "O3", direction = "output" },
      { name = "O4", direction = "output" },
   },
   
   infobox = [[
      Buttons` accepts input from the user, and outputs `1` or `0` depending if the button is pressed or not.
      ${image: __infobox_button_0}${image_sl: __infobox_button_1}
   ]],
   
   on_click = function(button: Component)
      button.data[1] = bnot(button.data[1])
   end,
   
   render = function(button: Component, scene: Scene, context: Context)
      scene:add_image("shadow_square", 1, 1, context)
      if not button or button.data[1] == 0 then
         scene:add_image("button_off", 0, 0, context)
      else
         scene:add_image("button_on", 0, 0, context)
      end
   end,
}

return button

-- vim:sw=3:st=3:sts=3:noexpandtab
