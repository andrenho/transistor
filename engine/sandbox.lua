Sandbox = {}
Sandbox.__index = Sandbox

function Sandbox.new()
   local self = setmetatable({}, Sandbox)
   self.boards = {}
   self.component_db = ComponentDB.new()
   return self
end

function Sandbox:add_board(w, h)
   local board = Board.new(w, h)
   self.boards[#self.boards+1] = board
   return board
end