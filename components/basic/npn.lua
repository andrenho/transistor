local SW0, IN, SW1, OUT = 1, 2, 3, 4

return {
   key = "__npn",
   type = "single_tile",
   can_rotate = true,
   
   pins = {
      { name = "SW0", direction = "input" },
      { name = "IN",  direction = "input" },
      { name = "SW1", direction = "output" },
      { name = "OUT", direction = "input" },
   },
   
   infobox = [[
      `NPN transistors` allow the passage of a signal if their input is `1`, and block it otherwise.
       There are two inputs, the input is considered `1` if one of the inputs is `1`.
       ${image: __infobox_npn_0}${image: __infobox_npn_1}
       Press `R` to rotate the component.
   ]],
   
   simulate = function(npn)
      npn.pin[OUT] = band(npn.pin[IN], bor(npn.pin[SW0], npn.pin[SW1]))
   end,
   
   render = function(npn, G, x, y)
      G:render_image('shadow_rect', x+1, y+1)
      G:render_image('npn', x, y)
   end,
}
