return {
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
   
   --[[
   simulate = function(led)
      led.data[1] = bor(led.pin[1], led.pin[2], led.pin[3], led.pin[4])
   end,
   ]]
   
   render = function(led, G, x, y, context)
      G:render_image('shadow_circle', x+1, y+1)
      if not led or led.data[1] == 0 then
         G:render_image('led_off', x, y, context)
      else
         G:render_image('led_on', x, y, context)
      end
   end,
}
