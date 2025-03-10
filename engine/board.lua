Board = {}
Board.__index = Board

function Board.new(w, h)
   local self = setmetatable({}, Board)
   self.w = w
   self.h = h
   self.wires = {}
   self.components = {}
   return self
end

function Board:wire(pos)
   return self.wires[pos:hash()]
end

function Board:add_wire(pos, wire)
   if pos.x < 0 or pos.y < 0 or pos.x >= self.w or pos.y >= self.h then return end
   self.wires[pos:hash()] = wire
   
   -- TODO - remove wires under ic
end

function Board:add_wires(pos1, pos2, orientation, wire)
   for _,pos in ipairs(Position.a_to_b(pos1, pos2, orientation)) do
      if pos.x >= 0 or pos.y >= 0 or pos.x < self.w or pos.y < self.h then
         self.wires[pos:hash()] = wire
      end
   end
end

function Board:clear_tile(pos)
   assert(pos.dir == CENTER)
   
   -- TODO clear component
   
   -- clear wires
   self.wires[P(pos.x, pos.y, N):hash()] = nil
   self.wires[P(pos.x, pos.y, E):hash()] = nil
   self.wires[P(pos.x, pos.y, S):hash()] = nil
   self.wires[P(pos.x, pos.y, W):hash()] = nil
end