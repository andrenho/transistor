local Position = require('board.geo.position')

local record Rect
   top_left: Position
   bottom_right: Position
end

function Rect.new(p1: Position | integer, p2: Position | integer, p3?: integer, p4?: integer) : Rect
   local rect : Rect = setmetatable({}, { __index = Rect })
   if p3 ~= nil and p4 ~= nil then
      rect.top_left = P(p1 as integer, p2 as integer)
      rect.bottom_right = P(p3, p4)
   else
      rect.top_left = p1 as Position
      rect.bottom_right = p2 as Position
   end
   return rect
end

function Rect:contains(pos: Position) : boolean
   return pos.x >= self.top_left.x and pos.x <= self.bottom_right.x and
          pos.y >= self.top_left.y and pos.y <= self.bottom_right.y
end

function Rect:fully_inside_of(rect: Rect) : boolean
   if self.top_left.x < rect.top_left.x or self.top_left.y < rect.top_left.y then
      return false
   end
   if self.bottom_right.x > rect.bottom_right.x or self.bottom_right.y > rect.bottom_right.y then
      return false
   end
   return true
end

function Rect:intersects_with(rect: Rect): boolean
   if self.bottom_right.x < rect.top_left.x or rect.bottom_right.x < self.top_left.x then
      return false
   end
   if self.bottom_right.y < rect.top_left.y or rect.bottom_right.y < self.top_left.y then
      return false
   end
   return true
end

return Rect

-- vim:sw=3:st=3:sts=3:noexpandtab
