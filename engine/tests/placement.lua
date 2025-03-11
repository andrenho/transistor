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

print("Single tile placement")

do
   print(" - Place component in circuit")
   local board = Sandbox.new():add_board(20, 10)
   board:add_component("__button", P(5, 5), N)
   assert(board:component(P(5, 5)).def.key == "__button")
end

do
   print(" - Don't place outside of circuit bounds")
   local board = Sandbox.new():add_board(20, 10)
   board:add_component("__button", P(20, 5), N)
   assert(board:component(P(20, 5)) == nil)
end

do
   print(" - Don't place single-tile component on top of another single-tile component")
   local board = Sandbox.new():add_board(20, 10)
   board:add_component("__button", P(1, 1), N)
   board:add_component("__led", P(1, 1), N)
   assert(board:component(P(1, 1)).def.key == "__button")
end

do
   print(" - Don't place single-tile component on top of IC component")
   local board = Sandbox.new():add_board(20, 10)
   board:add_component("__or_2i", P(1, 1), N)
   board:add_component("__led", P(2, 2), N)
   assert(board:component(P(1, 1)).def.key == "__or_2i")
end

do
   print(" - Remove component")
   local board = Sandbox.new():add_board(20, 10)
   board:add_component("__button", P(1, 1), N)
   board:clear_tile(P(1, 1))
   assert(board:component(P(1, 1)) == nil)
end

print("IC placement")

do
   print(" - Query IC in a different tile")
   local board = Sandbox.new():add_board(20, 10)
   board:add_component("__or_2i", P(2, 2), N)
   assert(board:component(P(1, 1)).def.key == "__or_2i")
end

do
   print(" - Don't place outside of circuit bounds")
   local board = Sandbox.new():add_board(10, 10)
   board:add_component("__or_2i", P(11, 1), N)
   assert(board:component(P(11, 1)) == nil)
end

do
   print(" - Don't place any part of IC outside of circuit bounds")
   local board = Sandbox.new():add_board(20, 10)
   board:add_component("__or_2i", P(1, 9), N)
   assert(board:component(P(1, 9)) == nil)
end

do
   print(" - Don't place IC over single-tile component")
   local board = Sandbox.new():add_board(20, 10)
   board:add_component("__button", P(2, 2), N)
   board:add_component("__or_2i", P(1, 1), N)
   assert(board:component(P(1, 1)) == nil)
end

do
   print(" - Remove IC")
   local board = Sandbox.new():add_board(20, 10)
   board:add_component("__or_2i", P(1, 1), N)
   board:clear_tile(P(2, 2))
   assert(board:component(P(1, 1)) == nil)
   assert(board:component(P(2, 2)) == nil)
end

do
   print(" - Don't place IC over another IC")
   local board = Sandbox.new():add_board(20, 10)
   board:add_component("__or_2i", P(2, 2), N)
   board:add_component("__or_2i", P(1, 1), N)
   assert(board:component(P(0, 0)) == nil)
end

do
   print(" - Overwrite wire on placement")
   local board = Sandbox.new():add_board(20, 10)
   board:add_wires(P(0, 1), P(4, 1), HORIZONTAL, { WIDTH_1, LAYER_TOP })
   board:add_component("__or_2i", P(2, 1), N)
   
   local i = 0; for _,_ in pairs(board.wires) do i = i + 1 end; print(i); assert(i == 4)
   assert(board:wire(P(0, 1, E)) ~= nil)
   assert(board:wire(P(1, 1, W)) ~= nil)
   assert(board:wire(P(3, 1, E)) ~= nil)
   assert(board:wire(P(4, 1, W)) ~= nil)
end
