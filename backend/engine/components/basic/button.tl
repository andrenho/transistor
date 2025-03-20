local ComponentDef = require("components.componentdef")
local Component = require("components.component")
local Graphics = require("graphics.graphics")
local Context = require("graphics.context")
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
   
   render = function(button: Component, G: Graphics, x: integer, y: integer, context: Context)
      G:render_image('shadow_square', x+1, y+1)
      if not button or button.data[1] == 0 then
         G:render_image('button_off', x, y, context)
      else
         G:render_image('button_on', x, y, context)
      end
   end,
}

return button

-- vim:sw=3:st=3:sts=3:noexpandtab
