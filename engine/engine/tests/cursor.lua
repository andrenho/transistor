print("Cursor")

do
   print(" - Use cursor")
   local board = Sandbox.new():add_board(20, 10)
   local cursor = board.cursor
   
   -- add wire
   cursor:set_pointer(P(1, 1))
   cursor:key_press("w")
   cursor:set_pointer(P(3, 1))
   cursor:key_release("w")
   
   -- add components
   cursor:set_pointer(P(1, 1))
   cursor:key_press("b")
   cursor:set_pointer(P(3, 1))
   cursor:key_press("l")
   
   assert(board:wire(P(2, 1, W)) ~= nil)
   assert(board:component(P(1, 1)).def.key == "__button")
   assert(board:component(P(3, 1)).def.key == "__led")
end