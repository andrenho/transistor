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
   
   on_click = function(button)
      button.data[1] = bit.bnot(button.data[1])
   end,
   
   simulate = function(button)
      for i=1,#button.pin do button.pin[i] = button.data[1] end
   end,
   
   -- render the component on the screen
   render = function(button, G, x, y, context)
      G.render_image('button_off', x, y, context)
   end,
}