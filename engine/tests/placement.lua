print("Wire placement")

do
   print(" - Place wire in circuit")
   local board = Sandbox.new():add_board(20, 10)
   board:add_wire(P(10, 1, E), { width = 1, layer = TOP })
   assert(board:wire(P(10, 1, E)) ~= nil)
end

do
   print(" - Don't place wire outside of circuit bounds")
   local board = Sandbox.new():add_board(20, 10)
   board:add_wire(P(20, 1, E), { WIDTH_1, LAYER_TOP })
   assert(board:wire(P(20, 1, E)) == nil)
end

do
   print(" - Remove wire")
   local board = Sandbox.new():add_board(20, 10)
   board:add_wires(P(1, 1), P(3, 1), HORIZONTAL, { WIDTH_1, LAYER_TOP })
   board:clear_tile(P(2, 1))
   assert(board:wire(P(1, 1, E)) ~= nil)
   assert(board:wire(P(2, 1, W)) == nil)
   assert(board:wire(P(2, 1, E)) == nil)
   assert(board:wire(P(3, 1, W)) ~= nil)
end

-- TODO