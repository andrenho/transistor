Sandbox = {}

function Sandbox.new()
   local self = setmetatable({}, Sandbox)
   self.boards = {}
   self.component_db = ComponentDB.new()
   return self
end