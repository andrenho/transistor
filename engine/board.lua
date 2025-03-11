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

function Board:component(pos)
   for _,component in ipairs(self.components) do
      if component:rect():contains(pos) then
         return component
      end
   end
   return nil
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
   
   local rect = def:rect(pos, dir)
   
   -- inside the board?
   if not rect:fully_inside_of(Rect.new(0, 0, self.w-1, self.h-1)) then
      return
   end
   
   -- is there another component there?
   for _,comp in ipairs(self.components) do
      if comp:rect():intersects_with(rect) then
         return
      end
   end
   
   -- initialize component
   local component = Component.new(def, pos, dir)
   self.components[#self.components+1] = component
   
   -- remove wires underneath
   if component.def.type ~= "single_tile" then
      self:remove_wires_for_ic(rect)
   end
end

function Board:clear_tile(pos)
   assert(pos.dir == CENTER)
   
   -- clear component
   for i,component in ipairs(self.components) do
      if component:rect():contains(pos) then
         table.remove(self.components, i)
         break
      end
   end
   
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
   -- remove sides
   for y = rect.top_left.y, rect.bottom_right.y do  -- left
      self.wires[P(rect.top_left.x, y, N):hash()] = nil
      self.wires[P(rect.top_left.x, y, S):hash()] = nil
      self.wires[P(rect.top_left.x, y, E):hash()] = nil
   end
   for y = rect.top_left.y, rect.bottom_right.y do  -- right
      self.wires[P(rect.bottom_right.x, y, N):hash()] = nil
      self.wires[P(rect.bottom_right.x, y, S):hash()] = nil
      self.wires[P(rect.bottom_right.x, y, W):hash()] = nil
   end
   for x = rect.top_left.x, rect.bottom_right.x do  -- top
      self.wires[P(x, rect.top_left.y, S):hash()] = nil
      self.wires[P(x, rect.top_left.y, W):hash()] = nil
      self.wires[P(x, rect.top_left.y, E):hash()] = nil
   end
   for x = rect.top_left.x, rect.bottom_right.x do  -- bottom
      self.wires[P(x, rect.bottom_right.y, N):hash()] = nil
      self.wires[P(x, rect.bottom_right.y, W):hash()] = nil
      self.wires[P(x, rect.bottom_right.y, E):hash()] = nil
   end
   
   -- remove center
   for x = rect.top_left.x + 1, rect.bottom_right.x - 1 do
      for y = rect.top_left.y + 1, rect.bottom_right.y - 1 do
         self.wires[P(x, y, N):hash()] = nil
         self.wires[P(x, y, S):hash()] = nil
         self.wires[P(x, y, W):hash()] = nil
         self.wires[P(x, y, E):hash()] = nil
      end
   end
end