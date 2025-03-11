Sandbox = {}
Sandbox.__index = Sandbox

function Sandbox.new()
   local self = setmetatable({}, Sandbox)
   self.boards = {}
   self.component_db = ComponentDB.new()
   return self
end

function Sandbox:add_board(w, h)
   local board = Board.new(w, h, self)
   self.boards[#self.boards+1] = board
   return board
end

function Sandbox:take_snapshot(include_db)
   local snap = { boards = {} }
   for i,board in ipairs(self.boards) do
      snap.boards[i] = board:take_snapshot()
   end
   if include_db then
      snap.component_db = self.component_db:take_snapshot()
   end
   return snap
end