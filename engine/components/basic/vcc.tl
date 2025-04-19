require "ui.scene"
require "ui.context"
require "components.componentdef"
require "components.component"

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
   
   render = function(_: Component | nil, scene: Scene, context: Context)
      scene:add_image("shadow_square", 1, 1, context)
      scene:add_image("vcc", 0, 0, context)
   end,
}

return vcc

-- vim:sw=3:st=3:sts=3:noexpandtab
