Board = {}

function Board.new(w, h)
   local self = setmetatable({}, Board)
   self.w = w
   self.h = h
   return self
end
