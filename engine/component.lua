Component = {}
Component.__index = Component

function Component.new(def, pos, dir)
   local self = setmetatable({}, Component)
   self.def = def
   self.position = pos
   self.direction = dir or N
   self.data = native_array(def.data_size)
   self.pin = native_array(#def.pins)
   return self
end

function Component:pin_positions()
   return self.def:pin_positions(self.position, self.direction)
end

function Component:rect()
   return self.def:rect(self.position, self.direction)
end

function Component:on_click()
   if self.def.on_click then
      self.def.on_click(self)
   end
end