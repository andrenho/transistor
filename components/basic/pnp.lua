local SW0, IN, SW1, OUT = 1, 2, 3, 4

return {
   key = "__pnp",
   type = "single_tile",
   can_rotate = true,
   
   pins = {
      { name = "SW0", direction = "input" },
      { name = "IN",  direction = "input" },
      { name = "SW1", direction = "output" },
      { name = "OUT", direction = "input" },
   },
   
   infobox = [[
      `PNP transistors` allow the passage of a signal if their input is `0`, and block it otherwise.
       There are two inputs, the input is considered `1` if one of the inputs is `1`.
       ${image: __infobox_pnp_0}${image: __infobox_pnp_1}
       The PNP transistor also works as a diode, blocking the signal in one direction but not the other.
       ${image: __infobox_diode_0}${image_sl: __infobox_diode_1}
       Press `R` to rotate the component.
   ]],
   
   simulate = function(pnp)
      pnp.pin[OUT] = band(pnp.pin[IN], bnot(bor(pnp.pin[SW0], pnp.pin[SW1])))
   end,
   
   render = function(pnp, G, x, y)
      G:render_image('shadow_rect', x+1, y+1)
      G:render_image('pnp', x, y)
   end,
}
