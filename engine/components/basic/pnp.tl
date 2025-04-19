require "ui.scene"
require "ui.context"
require "components.componentdef"
require "components.component"

local pnp : ComponentDef = {
   key = "__pnp",
   type = "single_tile",
   can_rotate = true,
   
   pins = {
      { name = "SW0", direction = "input" },
      { name = "IN",  direction = "input" },
      { name = "SW1", direction = "input" },
      { name = "OUT", direction = "output" },
   },
   
   infobox = [[
      `PNP transistors` allow the passage of a signal if their input is `0`, and block it otherwise.
       There are two inputs, the input is considered `1` if one of the inputs is `1`.
       ${image: __infobox_pnp_0}${image: __infobox_pnp_1}
       The PNP transistor also works as a diode, blocking the signal in one direction but not the other.
       ${image: __infobox_diode_0}${image_sl: __infobox_diode_1}
       Press `R` to rotate the component.
   ]],
   
   render = function(_: Component | nil, scene: Scene, context: Context)
      scene:add_image("shadow_rect", 1, 1, context)
      scene:add_image("pnp", 0, 0, context)
   end,
}

return pnp

-- vim:sw=3:st=3:sts=3:noexpandtab
