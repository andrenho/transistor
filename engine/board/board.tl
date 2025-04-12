require 'device'
require "board.geo.direction"
require "board.geo.orientation"
require "event"
require "components.component"
require "components.componentdb"
require "components.componentdef"
require "board.geo.position"
require "board.geo.rect"
require "board.wire"

global type Sandbox

local TILE_SIZE = 16

local record BoardCursor
   record DrawingWire
	  starting_pos: Position
	  end_pos: Position
	  orientation: Orientation
	  orientation_defined: boolean
   end

   selected_direction: Direction
   selected_wire: Wire
   selected_component: string
   erasing: boolean
   wire: DrawingWire
end

global record Board is Device
   w: integer
   h: integer
   wires: {PosHash: Wire}
   components: {Component}
   cursor: BoardCursor
   component_db: ComponentDB

   __index: Board
end
Board.__index = Board

global record SerialBoard
   w: integer
   h: integer
   wires: {SerialPosition|SerialWire}
   components: {SerialComponent}
end

function Board.new(w: integer, h: integer, component_db: ComponentDB) : Board
   local self : Board = setmetatable({}, Board as metatable<Board>)
   self.x, self.y = 70, 70
   self.zoom = 2.0
   self.z_order = 100
   self.w, self.h = w, h
   self.wires = {}
   self.components = {}
   self.component_db = component_db
   self.cursor = {
	  selected_direction = N,
	  selected_component = "",
	  selected_wire = WR("top", "1"),
	  erasing = false,
   }
   assert(type(self.w) == 'number' and type(self.h) == 'number')
   return self
end

--
-- INFORMATION
--

function Board:wire(pos: Position) : Wire
   return self.wires[pos:hash()]
end

function Board:component(pos: Position) : Component
   for _,component in ipairs(self.components) do
      if component:rect():contains(pos) then
         return component
      end
   end
   return nil
end

function Board:size() : Size
   return {
	  w = math.floor((self.w + 4) * TILE_SIZE * self.zoom), 
	  h = math.floor((self.h + 4) * TILE_SIZE * self.zoom),
   }
end

function Board:rect() : Rect
   local sz = self:size()
   local gap = math.floor(8 * self.zoom)
   return Rect.new(self.x - 2*gap, self.y - 2*gap,
                   self.x + sz.w - 4*gap, self.y + sz.h - 4*gap)
end

--
-- CHANGES
--

function Board:remove_wires_for_ic(rect: Rect)
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

function Board:remove_wires_under_ics()
   for _,c in ipairs(self.components) do
      if c.def.type ~= "single_tile" then
         self:remove_wires_for_ic(c:rect())
      end
   end
end

function Board:add_wire(pos: Position, wire: Wire)
   assert(wire)
   if pos.x < 0 or pos.y < 0 or pos.x >= self.w or pos.y >= self.h then return end
   self.wires[pos:hash()] = wire
   self:remove_wires_under_ics()
end

function Board:add_wires(pos1: Position, pos2: Position, orientation: Orientation, wire: Wire)
   for _,pos in ipairs(Position.a_to_b(pos1, pos2, orientation)) do
      if pos.x >= 0 or pos.y >= 0 or pos.x < self.w or pos.y < self.h then
         self.wires[pos:hash()] = wire
      end
   end
   self:remove_wires_under_ics()
end

function Board:add_component(def: ComponentDef | string, pos: Position, dir: Direction)
   if type(def) == "string" then def = self.component_db:def(def as string) end
   assert(dir ~= CENTER)

   local rect = (def as ComponentDef):rect(pos, dir)

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
   local component = Component.new(def as ComponentDef, pos, dir)
   self.components[#self.components+1] = component

   -- remove wires underneath
   if component.def.type ~= "single_tile" then
      self:remove_wires_for_ic(rect)
   end
end

function Board:rotate_tile(pos: Position)
   local component = self:component(pos)
   if component and component.def.can_rotate then
      component.direction = Dir.rotation[component.direction]
   end
end

function Board:clear()
   self.wires = {}
   self.components = {}
end

function Board:clear_tile(pos: Position)
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

-- 
-- SIMULATION
--

function Board:simulate()
   for _,c in ipairs(self.components) do
	  if c.def.simulate then
		 c.def.simulate(c)
	  end
   end
end

--
-- EVENTS
--

function Board:do_event(event: Event, rel_mouse_x: integer, rel_mouse_y: integer) : boolean
   local recompile = false

   local pos = P(
	  math.floor(rel_mouse_x / TILE_SIZE / self.zoom),
	  math.floor(rel_mouse_y / TILE_SIZE / self.zoom),
	  Dir.direction_in_tile(rel_mouse_x / self.zoom / TILE_SIZE, rel_mouse_y / self.zoom / TILE_SIZE)
   )
   local cursor = self.cursor

   if event.type == "move_pointer" then
	  if cursor.wire then
		 local dx = cursor.wire.starting_pos.x - pos.x
		 local dy = cursor.wire.starting_pos.y - pos.y
		 if not cursor.wire.orientation_defined and (dx ~= 0 or dy ~= 0) then
            if math.abs(dx) > math.abs(dy) then
               cursor.wire.orientation = HORIZONTAL
            else
               cursor.wire.orientation = VERTICAL
            end
            cursor.wire.orientation_defined = true
         end
         
         if cursor.wire.orientation_defined and dx == 0 and dy == 0 then
            cursor.wire.orientation_defined = false
         end
		 cursor.wire.end_pos = pos
	  end
	  if cursor.erasing then
		 self:clear_tile(pos:center())
		 recompile = true
	  end
	  
   elseif event.type == "mouse_press" then
	  if event.button == "left" then
		 local component = self:component(pos:center())
		 if component then
			component:on_click()
		 elseif cursor.selected_component then -- and cursor.selected_component ~= "" then
			self:add_component(cursor.selected_component, pos:center(), cursor.selected_direction)
			recompile = true
		 end
	  end

   elseif event.type == "key_down" then
	  if event.key == "r" then
		 if self:component(pos:center()) then
			self:rotate_tile(pos:center())
			recompile = true
		 else
			cursor.selected_direction = Dir.rotation[cursor.selected_direction]
			recompile = true
		 end
	  elseif event.key == "v" then
		 self:add_component("__vcc", pos:center(), cursor.selected_direction)
		 recompile = true
	  elseif event.key == "b" then
		 self:add_component("__button", pos:center(), cursor.selected_direction)
		 recompile = true
	  elseif event.key == "l" then
		 self:add_component("__led", pos:center(), cursor.selected_direction)
		 recompile = true
	  elseif event.key == "p" then
		 self:add_component("__pnp", pos:center(), cursor.selected_direction)
		 recompile = true
	  elseif event.key == "n" then
		 self:add_component("__npn", pos:center(), cursor.selected_direction)
		 recompile = true
	  elseif event.key == "x" then
		 cursor.erasing = true
		 self:clear_tile(pos:center())
		 recompile = true
	  elseif event.key == "w" then
		 cursor.wire = {
			orientation_defined = true,
			orientation = HORIZONTAL,
			starting_pos = pos,
			end_pos = pos,
		 }
	  end

   elseif event.type == "key_up" then
	  if event.key == "x" then
		 cursor.erasing = false
	  elseif event.key == "w" then
		 if cursor.wire then
			if pos == cursor.wire.starting_pos and pos.dir ~= CENTER then
			   self:add_wire(pos, cursor.selected_wire)
			   recompile = true
			else
			   self:add_wires(cursor.wire.starting_pos, pos, cursor.wire.orientation, cursor.selected_wire)
			   recompile = true
			end
			cursor.wire = nil
		 end
	  end

   elseif event.type == "clear_board" then
	  self:clear()
	  recompile = true
   end

   return recompile
end

--
-- SERIALIZATION
--

function Board:serialize() : SerialBoard
   local data : SerialBoard = { w = self.w, h = self.h, wires = {}, components = {} }
   for pos_hash, wire in pairs(self.wires) do
	  data.wires[#data.wires + 1] = Position.unhash(pos_hash):serialize()
	  data.wires[#data.wires + 1] = wire:serialize()
   end
   for _, component in ipairs(self.components) do
	  data.components[#data.components + 1] = component:serialize()
   end
   return data
end

function Board.unserialize(data: SerialBoard, component_db: ComponentDB) : Board
   local board = Board.new(data.w, data.h, component_db)
   for i=1,#data.wires,2 do
	  board:add_wire(Position.unserialize(data.wires[i] as SerialPosition), Wire.unserialize(data.wires[i+1] as SerialWire))
   end
   for _,c in ipairs(data.components) do
	  board:add_component(c.def, Position.unserialize(c.pos), c.dir)
   end
   return board
end

-- vim:sw=3:st=3:sts=3:noexpandtab
