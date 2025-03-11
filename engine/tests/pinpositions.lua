print("Pin positions")

local custom_ic = {
   key = "my",
   type = "OVERWRITE",
   pins = {},  -- OVERWRITE
   
   name = "yyy",
   category = "digital",
   subcategory = "xxx"
}

function component(type, n_pins, ic_width)
   local my_ic = {}
   for k,v in pairs(custom_ic) do my_ic[k] = v end
   my_ic.type = type
   my_ic.ic_width = ic_width or 1
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


print(" - DIP component - 6 pins")

do
   print("   - Direction N")
   local c = component("ic_dip", 6)
   c.position = P(1, 1)
   c.direction = N
   
   local rect = c:rect()
   assert(rect.top_left == P(0, 0))
   assert(rect.bottom_right == P(2, 4))
   
   local pins = c:pin_positions()
   
   assert(#pins == 6)
   for i=1,6 do assert(pins[i].pin_no == i) end
   assert(pins[1].pos == P(0, 1))
   assert(pins[2].pos == P(0, 2))
   assert(pins[3].pos == P(0, 3))
   assert(pins[4].pos == P(2, 3))
   assert(pins[5].pos == P(2, 2))
   assert(pins[6].pos == P(2, 1))
end

do
   print("   - Direction E")
   local c = component("ic_dip", 6)
   c.position = P(1, 1)
   c.direction = E
   
   local rect = c:rect()
   assert(rect.top_left == P(0, 0))
   assert(rect.bottom_right == P(4, 2))
   
   local pins = c:pin_positions()
   
   assert(#pins == 6)
   for i=1,6 do assert(pins[i].pin_no == i) end
   assert(pins[1].pos == P(1, 2))
   assert(pins[2].pos == P(2, 2))
   assert(pins[3].pos == P(3, 2))
   assert(pins[4].pos == P(3, 0))
   assert(pins[5].pos == P(2, 0))
   assert(pins[6].pos == P(1, 0))
end

do
   print("   - Direction S")
   local c = component("ic_dip", 6)
   c.position = P(1, 1)
   c.direction = S
   
   local rect = c:rect()
   assert(rect.top_left == P(0, 0))
   assert(rect.bottom_right == P(2, 4))
   
   local pins = c:pin_positions()
   
   assert(#pins == 6)
   for i=1,6 do assert(pins[i].pin_no == i) end
   assert(pins[1].pos == P(2, 3))
   assert(pins[2].pos == P(2, 2))
   assert(pins[3].pos == P(2, 1))
   assert(pins[4].pos == P(0, 1))
   assert(pins[5].pos == P(0, 2))
   assert(pins[6].pos == P(0, 3))
end

do
   print("   - Direction W")
   local c = component("ic_dip", 6)
   c.position = P(1, 1)
   c.direction = W
   
   local rect = c:rect()
   assert(rect.top_left == P(0, 0))
   assert(rect.bottom_right == P(4, 2))
   
   local pins = c:pin_positions()
   
   assert(#pins == 6)
   for i=1,6 do assert(pins[i].pin_no == i) end
   assert(pins[1].pos == P(3, 0))
   assert(pins[2].pos == P(2, 0))
   assert(pins[3].pos == P(1, 0))
   assert(pins[4].pos == P(1, 2))
   assert(pins[5].pos == P(2, 2))
   assert(pins[6].pos == P(3, 2))
end

print(" - DIP component - 6 pins - width 2")

do
   print("   - Direction N")
   local c = component("ic_dip", 6, 2)
   c.position = P(1, 1)
   c.direction = N
   
   local rect = c:rect()
   assert(rect.top_left == P(0, 0))
   assert(rect.bottom_right == P(3, 4))
   
   local pins = c:pin_positions()
   
   assert(#pins == 6)
   for i=1,6 do assert(pins[i].pin_no == i) end
   assert(pins[1].pos == P(0, 1))
   assert(pins[2].pos == P(0, 2))
   assert(pins[3].pos == P(0, 3))
   assert(pins[4].pos == P(3, 3))
   assert(pins[5].pos == P(3, 2))
   assert(pins[6].pos == P(3, 1))
end

do
   print("   - Direction E")
   local c = component("ic_dip", 6, 2)
   c.position = P(1, 1)
   c.direction = E
   
   local rect = c:rect()
   assert(rect.top_left == P(0, 0))
   assert(rect.bottom_right == P(4, 3))
   
   local pins = c:pin_positions()
   
   assert(#pins == 6)
   for i=1,6 do assert(pins[i].pin_no == i) end
   assert(pins[1].pos == P(1, 3))
   assert(pins[2].pos == P(2, 3))
   assert(pins[3].pos == P(3, 3))
   assert(pins[4].pos == P(3, 0))
   assert(pins[5].pos == P(2, 0))
   assert(pins[6].pos == P(1, 0))
end

do
   print("   - Direction S")
   local c = component("ic_dip", 6, 2)
   c.position = P(1, 1)
   c.direction = S
   
   local rect = c:rect()
   assert(rect.top_left == P(0, 0))
   assert(rect.bottom_right == P(3, 4))
   
   local pins = c:pin_positions()
   
   assert(#pins == 6)
   for i=1,6 do assert(pins[i].pin_no == i) end
   assert(pins[1].pos == P(3, 3))
   assert(pins[2].pos == P(3, 2))
   assert(pins[3].pos == P(3, 1))
   assert(pins[4].pos == P(0, 1))
   assert(pins[5].pos == P(0, 2))
   assert(pins[6].pos == P(0, 3))
end

do
   print("   - Direction W")
   local c = component("ic_dip", 6, 2)
   c.position = P(1, 1)
   c.direction = W
   
   local rect = c:rect()
   assert(rect.top_left == P(0, 0))
   assert(rect.bottom_right == P(4, 3))
   
   local pins = c:pin_positions()
   
   assert(#pins == 6)
   for i=1,6 do assert(pins[i].pin_no == i) end
   assert(pins[1].pos == P(3, 0))
   assert(pins[2].pos == P(2, 0))
   assert(pins[3].pos == P(1, 0))
   assert(pins[4].pos == P(1, 3))
   assert(pins[5].pos == P(2, 3))
   assert(pins[6].pos == P(3, 3))
end

print(" - QUAD component - 8 pins")

do
   print("   - Direction N")
   local c = component("ic_quad", 8)
   c.position = P(1, 1)
   c.direction = N
   
   local rect = c:rect()
   assert(rect.top_left == P(0, 0))
   assert(rect.bottom_right == P(3, 3))
   
   local pins = c:pin_positions()
   
   assert(#pins == 8)
   for i=1,8 do assert(pins[i].pin_no == i) end
   assert(pins[1].pos == P(0, 1))
   assert(pins[2].pos == P(0, 2))
   assert(pins[3].pos == P(1, 3))
   assert(pins[4].pos == P(2, 3))
   assert(pins[5].pos == P(3, 2))
   assert(pins[6].pos == P(3, 1))
   assert(pins[7].pos == P(2, 0))
   assert(pins[8].pos == P(1, 0))
end

do
   print("   - Direction E")
   local c = component("ic_quad", 8)
   c.position = P(1, 1)
   c.direction = E
   
   local rect = c:rect()
   assert(rect.top_left == P(0, 0))
   assert(rect.bottom_right == P(3, 3))
   
   local pins = c:pin_positions()
   
   assert(#pins == 8)
   for i=1,8 do assert(pins[i].pin_no == i) end
   assert(pins[1].pos == P(2, 0))
   assert(pins[2].pos == P(1, 0))
   assert(pins[3].pos == P(0, 1))
   assert(pins[4].pos == P(0, 2))
   assert(pins[5].pos == P(1, 3))
   assert(pins[6].pos == P(2, 3))
   assert(pins[7].pos == P(3, 2))
   assert(pins[8].pos == P(3, 1))
end

do
   print("   - Direction S")
   local c = component("ic_quad", 8)
   c.position = P(1, 1)
   c.direction = S
   
   local rect = c:rect()
   assert(rect.top_left == P(0, 0))
   assert(rect.bottom_right == P(3, 3))
   
   local pins = c:pin_positions()
   
   assert(#pins == 8)
   for i=1,8 do assert(pins[i].pin_no == i) end
   assert(pins[1].pos == P(3, 2))
   assert(pins[2].pos == P(3, 1))
   assert(pins[3].pos == P(2, 0))
   assert(pins[4].pos == P(1, 0))
   assert(pins[5].pos == P(0, 1))
   assert(pins[6].pos == P(0, 2))
   assert(pins[7].pos == P(1, 3))
   assert(pins[8].pos == P(2, 3))
end

do
   print("   - Direction W")
   local c = component("ic_quad", 8)
   c.position = P(1, 1)
   c.direction = W
   
   local rect = c:rect()
   assert(rect.top_left == P(0, 0))
   assert(rect.bottom_right == P(3, 3))
   
   local pins = c:pin_positions()
   
   assert(#pins == 8)
   for i=1,8 do assert(pins[i].pin_no == i) end
   assert(pins[1].pos == P(1, 3))
   assert(pins[2].pos == P(2, 3))
   assert(pins[3].pos == P(3, 2))
   assert(pins[4].pos == P(3, 1))
   assert(pins[5].pos == P(2, 0))
   assert(pins[6].pos == P(1, 0))
   assert(pins[7].pos == P(0, 1))
   assert(pins[8].pos == P(0, 2))
end

