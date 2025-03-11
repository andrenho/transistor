Board = {}
Board.__index = Board

function Board.new(w, h, sandbox)
   local self = setmetatable({}, Board)
   self.w = w
   self.h = h
   self.wires = {}
   self.components = {}
   self.sandbox = sandbox
   return self
end

function Board:wire(pos)
   return self.wires[pos:hash()]
end

function Board:add_wire(pos, wire)
   if pos.x < 0 or pos.y < 0 or pos.x >= self.w or pos.y >= self.h then return end
   self.wires[pos:hash()] = wire
   self:remove_wires_under_ics()
end

function Board:add_wires(pos1, pos2, orientation, wire)
   for _,pos in ipairs(Position.a_to_b(pos1, pos2, orientation)) do
      if pos.x >= 0 or pos.y >= 0 or pos.x < self.w or pos.y < self.h then
         self.wires[pos:hash()] = wire
      end
   end
   self:remove_wires_under_ics()
end

function Board:add_component(key, pos, dir)
   assert(dir ~= CENTER)
   
   local def = self.sandbox.component_db:def(key)
   assert(def ~= nil, "Component '" .. key .. "' not found in database.")
   
   -- TODO is it inside the board
   
   -- TODO is there another component there
   
   -- initialize component
   local component = Component.new(def, pos, dir)
   self.components[#self.components+1] = component
   
   if component.def.type ~= "single_tile" then
      self:remove_wires_for_ic(component:rect())
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

function Board:remove_wires_under_ics()
   for _,c in ipairs(self.components) do
      if c.def.type == "single_tile" then
         self:remove_wires_for_ic(c:rect())
      end
   end
end

function Board:remove_wires_for_ic(rect)
   -- TODO
end