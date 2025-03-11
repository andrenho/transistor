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
   return (self.x * 10000) + self.y + dir_hash(self.dir)
end

function Position.a_to_b(a, b, orientation)
   
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
   
   local pos = {}
   if a == b then return { a } end
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
