Component = {}
Component.__index = Component

function Component.new(def, dir)
   local self = setmetatable({}, Component)
   self.def = def
   self.direction = dir or N
   -- TODO - data & pins
   return self
end

function Component:pin_positions()
   return self.def:pin_positions(self.position, self.direction)
end