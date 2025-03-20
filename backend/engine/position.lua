local _tl_compat; if (tonumber((_VERSION or ''):match('[%d.]*$')) or 0) < 5.3 then local p, m = pcall(require, 'compat53.module'); if p then _tl_compat = m end end; local assert = _tl_compat and _tl_compat.assert or assert; local ipairs = _tl_compat and _tl_compat.ipairs or ipairs; local math = _tl_compat and _tl_compat.math or math


local bit = require('bit')
local fdir = require('board.geo.dir')





local Position = {}







Position.__index = Position

function Position.new(x, y, dir)
   local self = setmetatable({}, Position)
   self.x = x
   self.y = y
   self.dir = dir or CENTER
   return self
end

P = Position.new

function Position.__eq(a, b)
   print("X")
   return a.x == b.x and a.y == b.y and a.dir == b.dir
end

function Position:hash()
   return bit.bor(bit.lshift(self.x, 20), bit.lshift(self.y, 8), fdir.hash[self.dir])
end

function Position.unhash(h)
   local dir = fdir.unhash[bit.band(h, 0xf)]
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
      pos[#pos + 1] = P(x1, y, E)
      for x = (x1 + 1), (x2 - 1) do
         pos[#pos + 1] = P(x, y, W)
         pos[#pos + 1] = P(x, y, E)
      end
      pos[#pos + 1] = P(x2, y, W)

      return pos
   end

   local function a_to_b_vertical(x, y1, y2)
      if y1 == y2 then return {} end
      assert(y2 > y1)

      local pos = {}
      pos[#pos + 1] = P(x, y1, S)
      for y = (y1 + 1), (y2 - 1) do
         pos[#pos + 1] = P(x, y, N)
         pos[#pos + 1] = P(x, y, S)
      end
      pos[#pos + 1] = P(x, y2, N)

      return pos
   end

   local pos
   local pos2
   if orientation == HORIZONTAL then
      pos = a_to_b_horizontal(math.min(a.x, b.x), math.max(a.x, b.x), a.y)
      pos2 = a_to_b_vertical(b.x, math.min(a.y, b.y), math.max(a.y, b.y))
   elseif orientation == VERTICAL then
      pos = a_to_b_vertical(a.x, math.min(a.y, b.y), math.max(a.y, b.y))
      pos2 = a_to_b_horizontal(math.min(a.x, b.x), math.max(a.x, b.x), b.y)
   end
   for _, p in ipairs(pos2) do pos[#pos + 1] = p end
   return pos
end

return Position
