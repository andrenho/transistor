-- use this skeleton as a template to create your own components

return {
   
   -- component name, also used as index (can't have duplicates)
   key = "my_component",
   
   -- type can be either 'single_tile' (up to 4 pins), 'ic_dip' (PDIP, two rows of pins) or 'ic_quad' (square, four rows of pins)
   type = "ic_dip",
   
   -- whether the component can be rotated (default: true)
   can_rotate = true,
   
   -- width of the IC (used only for 'ic_dip', deafult 1)
   ic_width = 1,
   
   -- number of bytes to store in 'data' (data can be accessed both in Lua and C)
   data_size = 256,
   
   -- list of pins, each pin has its own name (visible to the user), direction ('input' or 'output')
   -- and wire width (1 or 8 [bus], it's 1 by default)
   pins = {
      { name = "I1", direction = "output", wire_width = 1 },
      { name = "I2", direction = "output", wire_width = 1 },
      { name = "Q",  direction = "input",  wire_width = 1 },
      { name = "~Q", direction = "input",  wire_width = 1 },
   },
   
   -- code that runs on the component type initialization (when the component type is loaded)
   init = function(G)
      G.load_image_base64('my_image_name', 'MY_BASE_64')
   end,
   
   -- code the runs when each individual component is placed on the board
   init_component = function(component)
      for i=1,256 do component.data[i] = 0 end  -- remember that Lua arrays are 1-based
   end,
   
   -- code that runs when the component is clicked
   on_click = function(component)
      component.data[1] = 255
   end,
   
   -- core function that runs at each emulation second (maybe 100K times per second), needs to be highly optimized
   -- read the pin inputs, and write to the pin outpus
   simulate = function(component)
      component.data[2] = component.pin[2]
      component.pin[3] = bit.bnot(component.data[2])  -- 'bit' library is available
   end,
   
   -- render the component on the screen
   render = function(component, G, x, y, context)
      context.transparency = 0.5
      G.render_image('my_image_name', x, y, context)
   end,
}