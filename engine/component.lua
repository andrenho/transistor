Component = {}
Component.__index = Component

function Component.new(def, dir)
   local self = setmetatable({}, Component)
   self.def = def
   self.dir = dir or N
   -- TODO - data & pins
   return self
end