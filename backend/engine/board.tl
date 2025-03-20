local type Board = record
end

function Board.new(id: number, w: number, h: number, sandbox: Sandbox) : Board
   local self : Board = setmetatable({}, { __index = Board })
   return self
end

return Board
