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
   setmetatable(my_ic, ComponentDef)
   return Component.new(my_ic)
end

print(" - Single-tile 1 pin")

do
   print("   - Direction N")
   local c = component("single_tile", 1)
   c.position = P(1, 1)
   c.direction = N
   local pins = c:pin_positions()
   
   assert(#pins == 1)
   assert(pins[1].pos == P(1, 1, N))
end

do
   print("   - Direction E")
   local c = component("single_tile", 1)
   c.position = P(1, 1)
   c.direction = E
   local pins = c:pin_positions()
   
   assert(#pins == 1)
   assert(pins[1].pos == P(1, 1, E))
end

print(" - Single-tile 2 pin")

do
   print("   - Direction N")
   local c = component("single_tile", 2)
   c.position = P(1, 1)
   c.direction = N
   local pins = c:pin_positions()
   
   assert(#pins == 2)
   assert(pins[1].pos == P(1, 1, N))
   assert(pins[2].pos == P(1, 1, S))
end

do
   print("   - Direction E")
   local c = component("single_tile", 2)
   c.position = P(1, 1)
   c.direction = E
   local pins = c:pin_positions()
   
   assert(#pins == 2)
   assert(pins[1].pos == P(1, 1, E))
   assert(pins[2].pos == P(1, 1, W))
end

print(" - Single-tile 4 pin")

do
   print("   - Direction N")
   local c = component("single_tile", 4)
   c.position = P(1, 1)
   c.direction = N
   local pins = c:pin_positions()
   
   assert(#pins == 4)
   for i=1,4 do assert(pins[i].pin_no == i) end
   assert(pins[1].pos == P(1, 1, N))
   assert(pins[2].pos == P(1, 1, W))
   assert(pins[3].pos == P(1, 1, S))
   assert(pins[4].pos == P(1, 1, E))
end

do
   print("   - Direction E")
   local c = component("single_tile", 4)
   c.position = P(1, 1)
   c.direction = E
   local pins = c:pin_positions()
   
   assert(#pins == 4)
   for i=1,4 do assert(pins[i].pin_no == i) end
   assert(pins[1].pos == P(1, 1, E))
   assert(pins[2].pos == P(1, 1, N))
   assert(pins[3].pos == P(1, 1, W))
   assert(pins[4].pos == P(1, 1, S))
end

