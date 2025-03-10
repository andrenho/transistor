print("Pin positions")

local custom_ic = {
   key = "my",
   type = "OVERWRITE",
   ic_width = 1,
   pins = {},  -- OVERWRITE
   
   name = "yyy",
   category = "digital",
   subcategory = "xxx"
}

function component(type, n_pins)
   local my_ic = {}
   for k,v in pairs(custom_ic) do my_ic[k] = v end
   my_ic.type = type
   for i=1,n_pins do
      my_ic.pins[i] = { name = "x", direction = "output" }
   end
   return Component.new(my_ic)
end

do
   print(" - Single-tile 1 pin")
   local c = component("single_tile", 1)
   c.position = P(1, 1, N)
   local pins = c:pin_positions()
   
   assert(#pins == 1)
   assert(pins[1].pos == { 1, 1, E })
end