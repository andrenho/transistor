require "board.geo.direction"
require "board.geo.position"

local tnew = require "table.new"

global record PositionSet
    items: {integer: Position}
    size: integer
end

function PositionSet.new() : PositionSet
   local self : PositionSet = setmetatable({}, { __index = PositionSet })
   self.items = {}
   self.size = 0
   return self
end

function PositionSet.from_hashes(values: {integer})
   local self : PositionSet = setmetatable({}, { __index = PositionSet })
   self.items = tnew(0, #values) as {integer:Position}
   for _,v in ipairs(values) do
	  self.items[v] = Position.unhash(v)
   end
   self.size = #values
end

function PositionSet:add(pos: Position | integer, y?: integer, dir?: Direction)
   if y ~= nil then pos = P(pos as integer, y, dir) end
   if not self.items[(pos as Position):hash()] then
      self.size = self.size + 1
      self.items[(pos as Position):hash()] = pos as Position
   end
end

function PositionSet:contains(pos: Position | integer, y?: integer, dir?: Direction) : boolean
   if y ~= nil then pos = P(pos as integer, y, dir) end
   return self.items[(pos as Position):hash()] ~= nil
end

function PositionSet:remove(pos: Position | integer, y?: integer, dir?: Direction)
   if y ~= nil then pos = P(pos as integer, y, dir) end
   if self.items[(pos as Position):hash()] then
      self.size = self.size -1
      self.items[(pos as Position):hash()] = nil
   end
end

function PositionSet:first() : Position | nil
   for _,v in pairs(self.items) do return v end
   return nil
end

function PositionSet:len() : integer
   return self.size
end

-- vim:sw=3:st=3:sts=3:noexpandtab
