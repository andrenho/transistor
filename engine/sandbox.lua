Sandbox = {}
Sandbox.__index = Sandbox

function Sandbox.new()
   local self = setmetatable({}, Sandbox)
   self.boards = {}
   self.component_db = ComponentDB.new()
   return self
end

function Sandbox.from_snapshot(snap)
   local new_sandbox = Sandbox.new()
   if snap.component_db then
      new_sandbox.component_db:import_snapshot(snap.component_db)
   end
   for _,b in ipairs(snap.boards) do
      new_sandbox.boards[#new_sandbox.boards+1] = Board.from_snapshot(b, new_sandbox)
   end
   return new_sandbox
end

function Sandbox:add_board(w, h)
   local board = Board.new(w, h, self)
   self.boards[#self.boards+1] = board
   return board
end

function Sandbox:simulate_lua_components()
   for _,board in ipairs(self.boards) do
      board:simulate_lua_components()
   end
end

function Sandbox:take_snapshot(goal)
   assert(goal == "save" or goal == "scene")
   local snap = { boards = {} }
   for i,board in ipairs(self.boards) do
      snap.boards[i] = board:take_snapshot()
   end
   if goal == "save" then
      snap.component_db = self.component_db:take_snapshot()
   end
   return snap
end
