require "components.componentdef" 
require "components.componentdb" 
require "board.geo.position" 
require "board.geo.rect" 
require 'board.geo.pinpos' 
require "board.geo.direction" 

global record Component
   def: ComponentDef
   position: Position
   direction: Direction

   data: {integer}
   pins: {integer}

   __index: Component
end
Component.__index = Component

global record SerialComponent
   def: string
   pos: SerialPosition
   dir: Direction
end

function Component.new(def: ComponentDef, position: Position, direction: Direction) : Component
    local self : Component = setmetatable({}, Component as metatable<Component>)
	self.def = def
	self.position = position
	self.direction = direction
	self.data = Array.new(def.data_size)
	self.pins = Array.new(#def.pins)
    return self
end

function Component:serialize() : SerialComponent
   return {
	  def = self.def.key,
	  pos = self.position:serialize(),
	  dir = self.direction
   }
end

function Component.unserialize(data: SerialComponent, component_db: ComponentDB) : Component
   return Component.new(component_db:def(data.def), Position.unserialize(data.pos), data.dir)
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

-- vim:sw=3:st=3:sts=3:noexpandtab
