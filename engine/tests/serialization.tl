require 'sandbox'

print("Serialization")

do
   print(" - Serialize and deserialize")
   
   local sb = Sandbox.new()
   local board = sb.board
   board:add_component("__npn", P(1, 1), N)
   board:add_component("__or_2i", P(3, 1), S)
   board:add_wires(P(1, 1), P(3, 1), HORIZONTAL, WR("top", "1"))
   
   local data = sb:serialize()
   -- print(serpent.line(data, { comment=false, fatal=true }))
   local sb2 = Sandbox.unserialize(data)
   local board2 = sb2.board
   
   assert(board2:wire(P(2, 1, W)) ~= nil)
   assert(board2:component(P(1, 1)).def.key == "__npn")
   assert(board2:component(P(3, 1)).def.key == "__or_2i")
end
