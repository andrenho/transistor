print("Connected wires")

function contains(array, item)
   for _,value in ipairs(array) do if value == item then return true end end
   return false
end

do
   print(" - Simple wires")
   local positions = PositionSet.new()
   positions:add(1, 1, E)
   positions:add(2, 1, W)
   positions:add(2, 1, E)
   positions:add(3, 1, W)
   
   local groups = find_connected_wires(positions, nil)
   assert(groups:len() == 1)
   
   local ps = groups[0]
   assert(#ps == 4)
   assert(contains(ps, P(1, 1, E)))
   assert(contains(ps, P(2, 1, W)))
   assert(contains(ps, P(2, 1, E)))
   assert(contains(ps, P(3, 1, W)))
end