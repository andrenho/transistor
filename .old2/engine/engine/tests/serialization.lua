print("Serialization")

do
   print(" - Serialize and deserialize")
   
   local sandbox = Sandbox.new()
   local board = sandbox:add_board(20, 10)
   board:add_component("__npn", P(1, 1), N)
   board:add_component("__or_2i", P(3, 1), S)
   board:add_wires(P(1, 1), P(3, 1), HORIZONTAL, WR(LAYER_TOP, WIDTH_1))
   
   local snap = sandbox:take_snapshot("scene")
   -- print(serpent.line(snap, { comment=false, fatal=true }))
   sb2 = Sandbox.from_snapshot(snap)
   
   assert(board:wire(P(2, 1, W)) ~= nil)
   assert(board:component(P(1, 1)).def.key == "__npn")
   assert(board:component(P(3, 1)).def.key == "__or_2i")
end