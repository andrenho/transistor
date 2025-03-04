return {
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
   
   on_click = function(button)
      button.data[1] = bnot(button.data[1])
   end,
   
   simulate = function(button)
      button.pin[1] = button.data[1]
      button.pin[2] = button.data[1]
      button.pin[3] = button.data[1]
      button.pin[4] = button.data[1]
   end,
   
   render = function(button, G, x, y)
      G:render_image('shadow_square', x+1, y+1)
      if not button or button.data[1] == 0 then
         G:render_image('button_off', x, y)
      else
         G:render_image('button_on', x, y)
      end
   end,
}
