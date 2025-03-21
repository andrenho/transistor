require('board.geo.position')
local PositionSet = require('board.geo.positionset')

print("Connected wires")

do
   print(" - Simple wires")
   local positions = PositionSet.new()
   positions:add(1, 1, E)
   positions:add(2, 1, W)
   positions:add(2, 1, E)
   positions:add(3, 1, W)
   
   local groups = compiler.find_connected_wires(positions, {})
   assert(#groups == 1)
   
   local ps = groups[1]
   assert(#ps == 4)
   assert(contains(ps, P(1, 1, E)))
   assert(contains(ps, P(2, 1, W)))
   assert(contains(ps, P(2, 1, E)))
   assert(contains(ps, P(3, 1, W)))
end

do
   print(" - Two separate wires")
   
   local positions = PositionSet.new()
   positions:add(1, 1, E)
   positions:add(2, 1, W)
   positions:add(2, 1, E)
   positions:add(3, 1, W)
   
   positions:add(1, 2, E)
   positions:add(2, 2, W)
   positions:add(2, 2, E)
   positions:add(3, 2, W)
   
   local groups = compiler.find_connected_wires(positions, {})
   assert(#groups == 2)
   
   local ps = groups[1]
   assert(#ps == 4)
   assert(contains(ps, P(1, 2, E)))
   assert(contains(ps, P(2, 2, W)))
   assert(contains(ps, P(2, 2, E)))
   assert(contains(ps, P(3, 2, W)))
   
   ps = groups[2]
   assert(#ps == 4)
   assert(contains(ps, P(1, 1, E)))
   assert(contains(ps, P(2, 1, W)))
   assert(contains(ps, P(2, 1, E)))
   assert(contains(ps, P(3, 1, W)))
end

do
   print(" - Crossing wires")
   local positions = PositionSet.new()
   positions:add(1, 1, E)
   positions:add(2, 1, W)
   positions:add(2, 1, E)
   positions:add(3, 1, W)
   
   positions:add(2, 1, N)
   positions:add(2, 0, S)
   positions:add(2, 0, E)
   positions:add(3, 0, W)
   
   positions:add(2, 1, S)
   positions:add(2, 2, N)
   positions:add(2, 2, E)
   positions:add(3, 2, W)
   
   local groups = compiler.find_connected_wires(positions, {})
   assert(#groups == 1)
   
   local ps = groups[1]
   assert(#ps == 12)
   assert(contains(ps, P(2, 1, E)))
   assert(contains(ps, P(2, 1, W)))
   assert(contains(ps, P(2, 1, N)))
   assert(contains(ps, P(2, 1, S)))
end

do
   print(" - Single-tile component in middle of connection")
   local positions = PositionSet.new()
   positions:add(1, 1, W)
   positions:add(1, 1, S)
   
   local single_tile_pins = { P(1, 1) }
   local groups = compiler.find_connected_wires(positions, single_tile_pins)
   assert(#groups == 2)
   
   assert(#groups[1] == 1)
   assert(contains(groups[1], P(1, 1, S)))
   assert(#groups[2] == 1)
   assert(contains(groups[2], P(1, 1, W)))
end
