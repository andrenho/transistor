local ComponentDef = require("components.componentdef")
local Component = require("components.component")
local Graphics = require("graphics.graphics")
local Context = require("graphics.context")

local npn : ComponentDef = {
   key = "__npn",
   type = "single_tile",
   can_rotate = true,
   
   pins = {
      { name = "SW0", direction = "input" },
      { name = "IN",  direction = "input" },
      { name = "SW1", direction = "input" },
      { name = "OUT", direction = "output" },
   },
   
   infobox = [[
      `NPN transistors` allow the passage of a signal if their input is `1`, and block it otherwise.
       There are two inputs, the input is considered `1` if one of the inputs is `1`.
       ${image: __infobox_npn_0}${image: __infobox_npn_1}
       Press `R` to rotate the component.
   ]],
   
   render = function(_: Component, G: Graphics, x: integer, y: integer, context: Context)
      G:render_image('shadow_rect', x+1, y+1, context)
      G:render_image('npn', x, y, context)
   end,
}

return npn
