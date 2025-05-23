require "board.geo.direction"
require "board.geo.orientation"

local bit = require('bit')

--
-- Position
--

global record Position
   x: integer
   y: integer
   dir: Direction

   __index: Position
end
Position.__index = Position

global type SerialPosition = {integer, integer, Direction}

function Position.new(x: integer, y: integer, dir?: Direction) : Position
   local self : Position = setmetatable({}, Position as metatable<Position>)
   self.x = x
   self.y = y
   self.dir = dir or CENTER
   return self
end

function Position.unserialize(data: SerialPosition) : Position
   return Position.new(data[1], data[2], data[3] or "CENTER")
end

function Position:serialize() : SerialPosition
   if self.dir == "CENTER" then
	  return { self.x, self.y }
   else
	  return { self.x, self.y, self.dir }
   end
end

global P = Position.new

function Position.__eq(a: Position, b: Position) : boolean
   return a.x == b.x and a.y == b.y and a.dir == b.dir
end

function Position:hash() : integer
   return bit.band(bit.bor(bit.lshift(self.x, 20), bit.lshift(self.y, 8), Dir.hash[self.dir]), 0xffffffff)
end

function Position.unhash(h: integer): Position
   local dir = Dir.unhash[bit.band(h, 0xf)]
   local y = bit.band(bit.rshift(h, 8), 0xfff)
   local x = bit.rshift(h, 20)
   return P(x, y, dir)
end

function Position:center() : Position
   return P(self.x, self.y)
end

function Position.a_to_b(a: Position, b: Position, orientation: Orientation) : {Position}
   
   if a == b then return { P(a.x, a.y, a.dir == CENTER and E or a.dir) } end
   
   local function a_to_b_horizontal(x1: integer, x2: integer, y: integer) : {Position}
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
   
   local function a_to_b_vertical(x: integer, y1: integer, y2: integer) : {Position}
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
   
   local pos: {Position}
   local pos2: {Position}
   if orientation == HORIZONTAL then
      pos = a_to_b_horizontal(math.min(a.x, b.x), math.max(a.x, b.x), a.y)
      pos2 = a_to_b_vertical(b.x, math.min(a.y, b.y), math.max(a.y, b.y))
   elseif orientation == VERTICAL then
      pos = a_to_b_vertical(a.x, math.min(a.y, b.y), math.max(a.y, b.y))
      pos2 = a_to_b_horizontal(math.min(a.x, b.x), math.max(a.x, b.x), b.y)
   end
   for _,p in ipairs(pos2) do pos[#pos+1] = p end
   return pos
end

global type PosHash = integer

-- vim:sw=3:st=3:sts=3:noexpandtab
