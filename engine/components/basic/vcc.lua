return {
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
   
   simulate = function(vcc)
      vcc.pin[1] = 1
      vcc.pin[2] = 1
      vcc.pin[3] = 1
      vcc.pin[4] = 1
   end,
   
   render = function(vcc, G, x, y, context)
      G:render_image('shadow_square', x+1, y+1, context)
      G:render_image('vcc', x, y, context)
   end,
}
