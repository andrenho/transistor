local IN0, IN1, Q_, Q = 1, 2, 3, 4

return {
   key = "__or_2i",
   type = "ic_dip",
   can_rotate = true,
   
   name = "OR (2 inputs)",
   category = "logic_gates",
   subcategory = "OR gates",
   
   pins = {
      { name = "IN0", direction = "input" },
      { name = "IN1", direction = "input" },
      { name = "~Q",  direction = "output" },
      { name = "Q",   direction = "output" },
   },
   
   infobox = [[
      TODO
   ]],
   
   init = function()
      -- TODO - load icon
   end,
   
   simulate = function(comp)
      comp.pin[Q] = bor(c.pin[IN0], c.pin[IN1])
      comp.pin[Q_] = bnot(comp.pin[Q])
   end,
   
   render = function(comp, G, x, y, context)
      -- TODO
   end,
}
