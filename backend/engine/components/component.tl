local ComponentDef = require("components.componentdef")
local Position = require("board.geo.position")
local Rect = require("board.geo.rect")
local PinPos = require('board.geo.pinpos')
local type Direction = require("board.geo.direction")
require('decl.array')

local record Component
   def: ComponentDef
   position: Position
   direction: Direction

   data: {integer}

   __index: Component
end
Component.__index = Component

function Component.new(def: ComponentDef, position: Position, direction: Direction) : Component
    local self : Component = setmetatable({}, Component as metatable<Component>)
	self.def = def
	self.position = position
	self.direction = direction
	self.data = Array(def.data_size)
    return self
end

function Component:pin_positions() : {PinPos}
   return self.def:pin_positions(self.position, self.direction)
end

function Component:rect() : Rect
   return self.def:rect(self.position, self.direction)
end

function Component:on_click()
   if self.def.on_click then
      self.def.on_click(self)
   end
end

return Component

-- vim:sw=3:st=3:sts=3:noexpandtab
