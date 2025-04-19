require "ui.scene"
require "ui.context"
require "components.componentdef"
require "components.component"

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
   
   render = function(_: Component | nil, scene: Scene, context: Context)
      scene:add_image("shadow_rect", 1, 1, context)
      scene:add_image("npn", 0, 0, context)
   end,
}

return npn

-- vim:sw=3:st=3:sts=3:noexpandtab
