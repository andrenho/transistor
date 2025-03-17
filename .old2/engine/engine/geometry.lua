local bit = require('bit')

--
-- directions
--

CENTER = "CENTER"
N = "N"
E = "E"
S = "S"
W = "W"

function dir_hash(dir)
   if dir == CENTER then return 0 end
   if dir == N then return 1 end
   if dir == E then return 2 end
   if dir == S then return 3 end
   if dir == W then return 4 end
   assert(false)
end

function dir_unhash(n)
   if n == 0 then return CENTER end
   if n == 1 then return N end
   if n == 2 then return E end
   if n == 3 then return S end
   if n == 4 then return W end
   assert(false)
end

function dir_rotate_component(dir)
   if dir == CENTER then return CENTER end
   if dir == N then return E end
   if dir == E then return S end
   if dir == S then return W end
   if dir == W then return N end
   assert(false)
end

function dir_angle(dir)
   if dir == CENTER then return 0 end
   if dir == N then return 0 end
   if dir == E then return 90 end
   if dir == S then return 180 end
   if dir == W then return 270 end
end

--
-- orientation
--

HORIZONTAL = "horizontal"
VERTICAL   = "vertical"

--
-- Position
--

Position = {}
Position.__index = Position

function Position.new(x, y, dir)
   assert(dir == nil or dir == CENTER or dir == N or dir == W or dir == E or dir == S)
   
   local self = setmetatable({}, Position)
   self.x = x
   self.y = y
   self.dir = dir or CENTER
   return self
end

function Position.__eq(a, b)
   return a.x == b.x and a.y == b.y and a.dir == b.dir
end

function Position:hash()
   return bit.bor(bit.lshift(self.x, 20), bit.lshift(self.y, 8), dir_hash(self.dir))
end

function Position.unhash(h)
   local dir = dir_unhash(bit.band(h, 0xf))
   local y = bit.band(bit.rshift(h, 8), 0xfff)
   local x = bit.rshift(h, 20)
   return P(x, y, dir)
end

function Position.a_to_b(a, b, orientation)
   
   if a == b then return { P(a.x, a.y, a.dir == CENTER and E or a.dir) } end
   
   local function a_to_b_horizontal(x1, x2, y)
      if x1 == x2 then return {} end
      assert(x2 > x1)
      
      local pos = {}
      pos[#pos+1] = P(x1, y, E)
      for x=(x1+1),(x2-1) do
         pos[#pos+1] = P(x, y, W)
         pos[#pos+1] = P(x, y, E)
      end
      pos[#pos+1] = P(x2, y, W)
      
      return pos
   end
   
   local function a_to_b_vertical(x, y1, y2)
      if y1 == y2 then return {} end
      assert(y2 > y1)
      
      local pos = {}
      pos[#pos+1] = P(x, y1, S)
      for y=(y1+1),(y2-1) do
         pos[#pos+1] = P(x, y, N)
         pos[#pos+1] = P(x, y, S)
      end
      pos[#pos+1] = P(x, y2, N)
      
      return pos
   end
   
   local pos, pos2
   if orientation == HORIZONTAL then
      pos = a_to_b_horizontal(math.min(a.x, b.x), math.max(a.x, b.x), a.y)
      pos2 = a_to_b_vertical(b.x, math.min(a.y, b.y), math.max(a.y, b.y), a.y)
   elseif orientation == VERTICAL then
      pos = a_to_b_vertical(a.x, math.min(a.y, b.y), math.max(a.y, b.y))
      pos2 = a_to_b_horizontal(math.min(a.x, b.x), math.max(a.x, b.x), b.y)
   end
   for _,p in ipairs(pos2) do pos[#pos+1] = p end
   return pos
end

P = Position.new

--
-- Position set
--

PositionSet = {}
PositionSet.__index = PositionSet

function PositionSet.new()
   local self = setmetatable({}, PositionSet)
   self.items = {}
   self.size = 0
   return self
end

function PositionSet:add(pos, y, dir)
   if y ~= nil then pos = P(pos, y, dir) end
   if not self.items[pos:hash()] then
      self.size = self.size + 1
      self.items[pos:hash()] = pos
   end
end

function PositionSet:contains(pos, y, dir)
   if y ~= nil then pos = P(pos, y, dir) end
   return self.items[pos:hash()] ~= nil
end

function PositionSet:remove(pos, y, dir)
   if y ~= nil then pos = P(pos, y, dir) end
   if self.items[pos:hash()] then
      self.size = self.size -1
      self.items[pos:hash()] = nil
   end
end

function PositionSet:first()
   for _,v in pairs(self.items) do return v end
end

function PositionSet:len()
   return self.size
end

--
-- Rect
--

Rect = {}
Rect.__index = Rect

function Rect.new(p1, p2, p3, p4)
   local rect = setmetatable({}, Rect)
   if p3 ~= nil and p4 ~= nil then
      rect.top_left = P(p1, p2)
      rect.bottom_right = P(p3, p4)
   else
      rect.top_left = p1
      rect.bottom_right = p2
   end
   return rect
end

function Rect:contains(pos)
   return pos.x >= self.top_left.x and pos.x <= self.bottom_right.x and
          pos.y >= self.top_left.y and pos.y <= self.bottom_right.y
end

function Rect:fully_inside_of(rect)
   if self.top_left.x < rect.top_left.x or self.top_left.y < rect.top_left.y then
      return false
   end
   if self.bottom_right.x > rect.bottom_right.x or self.bottom_right.y > rect.bottom_right.y then
      return false
   end
   return true
end

function Rect:intersects_with(rect)
   if self.bottom_right.x < rect.top_left.x or rect.bottom_right.x < self.top_left.x then
      return false
   end
   if self.bottom_right.y < rect.top_left.y or rect.bottom_right.y < self.top_left.y then
      return false
   end
   return true
end
