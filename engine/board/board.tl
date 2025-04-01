local type Device = require 'device'
local type Direction = require "board.geo.direction"
local type Orientation = require "board.geo.orientation"
local type Scene = require "ui.scene"
local type Context = require "ui.context"
local type Event = require "event"

global type Sandbox

local fdir = require "board.geo.dir"
local Component = require "components.component"
local ComponentDB = require "components.componentdb"
local ComponentDef = require "components.componentdef"
local Position = require "board.geo.position"
local Rect = require "board.geo.rect"
local Wire = require "board.wire"

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

local record Board is Device
   w: integer
   h: integer
   wires: {PosHash: Wire}
   components: {Component}
   cursor: BoardCursor
   component_db: ComponentDB

   __index: Board
end

Board.__index = Board

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
      component.direction = fdir.rotation[component.direction]
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
-- EVENTS
--

function Board:do_event(event: Event, rel_mouse_x: integer, rel_mouse_y: integer)
   local pos = P(
	  math.floor(rel_mouse_x / TILE_SIZE / self.zoom),
	  math.floor(rel_mouse_y / TILE_SIZE / self.zoom)
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
		 self:clear_tile(pos)
	  end
	  
   elseif event.type == "mouse_press" then
	  if event.button == "left" then
		 local component = self:component(pos)
		 if component then
			component:on_click()
		 elseif cursor.selected_component then
			self:add_component(cursor.selected_component, pos, cursor.selected_direction)
		 end
	  end

   elseif event.type == "key_down" then
	  if event.key == "r" then
		 if self:component(pos) then
			self:rotate_tile(pos)
		 else
			cursor.selected_direction = fdir.rotation[cursor.selected_direction]
		 end
	  elseif event.key == "v" then self:add_component("__vcc", pos, cursor.selected_direction)
	  elseif event.key == "b" then self:add_component("__button", pos, cursor.selected_direction)
	  elseif event.key == "l" then self:add_component("__led", pos, cursor.selected_direction)
	  elseif event.key == "p" then self:add_component("__pnp", pos, cursor.selected_direction)
	  elseif event.key == "n" then self:add_component("__npn", pos, cursor.selected_direction)
	  elseif event.key == "x" then
		 cursor.erasing = true
		 self:clear_tile(pos)
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
			else
			   self:add_wires(cursor.wire.starting_pos, pos, cursor.wire.orientation, cursor.selected_wire)
			end
			cursor.wire = nil
		 end
	  end
   end
end

-- 
-- RENDERING
--

function Board:add_image(scene: Scene, image: string, x: integer, y: integer, context?: Context)
   scene:add_image(image, math.floor(x * TILE_SIZE + (self.x / self.zoom)), math.floor(y * TILE_SIZE + (self.y / self.zoom)), context)
end

function Board:__draw_board(scene: Scene)
   self:add_image(scene, "board_top_left", -2, -2)
   self:add_image(scene, "board_top_right", self.w, -2)
   self:add_image(scene, "board_bottom_left", -2, self.h)
   self:add_image(scene, "board_bottom_right", self.w, self.h)
   for x=0,self.w-1 do
	  self:add_image(scene, "board_top", x, -2)
	  self:add_image(scene, "board_bottom", x, self.h)
   end
   for y=0,self.h-1 do
	  self:add_image(scene, "board_left", -2, y)
	  self:add_image(scene, "board_right", self.w, y)
   end
   for x=0,self.w-1 do
	  for y=0,self.h-1 do
		 self:add_image(scene, "tile", x, y)
	  end
   end
end

function Board:__draw_wires(scene: Scene)
   local wire_image = function(pos: Position, wire: Wire) : string
	  return "wire_" .. wire.layer .. "_off_" .. pos.dir:lower() .. "_" .. wire.width
   end
   -- wires
   for pos_hash, wire in pairs(self.wires) do
	  local pos = Position.unhash(pos_hash)
	  self:add_image(scene, wire_image(pos, wire), pos.x, pos.y)
   end
   --temporary wire
   if self.cursor.wire then
	  local w = self.cursor.wire
	  for _,pos in ipairs(Position.a_to_b(w.starting_pos, w.end_pos, w.orientation)) do
		 self:add_image(scene, wire_image(pos, self.cursor.selected_wire), pos.x, pos.y, { opacity = .5 })
	  end
   end
end

function Board:__draw_components(scene: Scene)
   for _, component in ipairs(self.components) do
      local context = { 
		 position = { 
			x = math.floor(component.position.x * TILE_SIZE + (self.x / self.zoom)),
			y = math.floor(component.position.y * TILE_SIZE + (self.y / self.zoom)),
		 },
		 rotation = fdir.angle[component.direction]
	  }
	  component.def.render(component, scene, context)
   end
end

function Board:render() : Scene
   local scene = Scene.new()
   scene:push_context({ zoom = self.zoom })
   self:__draw_board(scene)
   self:__draw_wires(scene)
   self:__draw_components(scene)
   return scene
end

--[[
--
-- SERIALIZATION
--

function Board:take_snapshot()
   local snap = { w = self.w, h = self.h, wires = {}, components = {}, id = self.id }
   for pos_hash, wire in pairs(self.wires) do
      local pos = Position.unhash(pos_hash)
      snap.wires[#snap.wires+1] = { pos.x, pos.y, pos.dir, wire.layer .. wire.width, self:pos_hash_c(pos), false }
   end
   if self.cursor.wire.drawing then
      for _,pos in ipairs(Position.a_to_b(self.cursor.wire.starting_pos, self.cursor.pos, self.cursor.wire.orientation)) do
         snap.wires[#snap.wires+1] = { pos.x, pos.y, pos.dir, self.cursor.selected_wire.layer .. self.cursor.selected_wire.width, self:pos_hash_c(pos), true }
      end
   end
   for i, component in ipairs(self.components) do
      snap.components[i] = { component.position.x, component.position.y, component.direction, component.def.key, self:pos_hash_c(component.position), component.def.data_size, false }
   end
   return snap
end

function Board.from_snapshot(snap, sandbox)
   local board = Board.new(snap.id, snap.w, snap.h, sandbox)
   for _,w in ipairs(snap.wires) do
      board:add_wire(P(w[1], w[2], w[3]), WR(w[4]:sub(1, 1), w[4]:sub(2, 2)))
   end
   for _,c in ipairs(snap.components) do
      board:add_component(c[4], P(c[1], c[2]), c[3])
   end
   return board
end

function Board:simulate_lua_components()
   for _,c in ipairs(self.components) do
      if c.def.simulate then
         c.def.simulate(c)
      end
   end
end

function Board:render_components(G: Graphics, tile_size: integer)
   for _,c in ipairs(self.components) do
      local x = c.position.x * tile_size
      local y = c.position.y * tile_size
      local context : Context = {}
      context.rotation = fdir.angle[c.direction]
      c.def.render(c, G, x, y, context)
   end
end
]]

return Board

-- vim:sw=3:st=3:sts=3:noexpandtab
