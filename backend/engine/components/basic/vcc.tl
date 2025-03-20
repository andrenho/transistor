local ComponentDef = require("components.componentdef")
local Component = require("components.component")
local Graphics = require("graphics.graphics")
local Context = require("graphics.context")

local vcc : ComponentDef = {
   key = "__vcc",
   type = "single_tile",
   can_rotate = false,
   
   pins = {
      { name = "O1", direction = "output" },
      { name = "O2", direction = "output" },
      { name = "O3", direction = "output" },
      { name = "O4", direction = "output" },
   },
   
   infobox = [[
      `VCC` is a component that generate an output of `1` in all pins.
      ${image: __infobox_vcc}
   ]],
   
   render = function(_: Component, G: Graphics, x: integer, y: integer, context: Context)
      G:render_image('shadow_square', x+1, y+1, context)
      G:render_image('vcc', x, y, context)
   end,
}

return vcc
