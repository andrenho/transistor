require "ui.scene"
require "ui.context"
require "components.componentdef"
require "components.component"

local led : ComponentDef = {
   key = "__led",
   type = "single_tile",
   can_rotate = false,
   data_size = 1,
   
   pins = {
      { name = "I1", direction = "input" },
      { name = "I2", direction = "input" },
      { name = "I3", direction = "input" },
      { name = "I4", direction = "input" },
   },
   
   infobox = [[
      `LEDs` are turned on or off, depending if the input is `0` or `1`.
       If there are multiple inputs, the LED is turned on if one of the inputs is `1`.
       ${image: __infobox_led_0}${image_sl: __infobox_led_1}
   ]],
   
   render = function(led: Component | nil, scene: Scene, context: Context)
      scene:add_image("shadow_circle", 1, 1, context)
      if not led or (led as Component).data[1] == 0 then
		   scene:add_image("led_off", 0, 0, context)
      else
		   scene:add_image("led_on", 0, 0, context)
      end
   end,
}

return led

-- vim:sw=3:st=3:sts=3:noexpandtab
