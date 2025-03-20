local type Device = require('device')

local record Board is Device
   w: number
   h: number
end

Board.name = "board"

function Board.new(...: any) : Device
   assert(#{...} == 2)
   local self : Board = setmetatable({}, { __index = Board })
   self.w, self.h = ... as (number, number)
   assert(type(self.w) == 'number' and type(self.h) == 'number')
   return self
end

return Board

-- vim:sw=3:st=3:sts=3:noexpandtab
