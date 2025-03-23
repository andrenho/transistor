local type Direction = require "board.geo.direction"
local type Orientation = require "board.geo.orientation"
local type Scene = require "ui.scene"
local type Instruction = require "ui.instruction"

global type Sandbox
global type BoardId = integer

local fdir = require "board.geo.dir"
local Component = require "components.component"
local ComponentDef = require "components.componentdef"
local Position = require "board.geo.position"
local Rect = require "board.geo.rect"
local Wire = require "board.wire"

local record Board
   -- fields
   w: integer
   h: integer
   wires: {PosHash: Wire}
   components: {Component}

   -- fields set externally
   id: BoardId
   x: integer
   y: integer
   z_order: integer
   sandbox: Sandbox

   __index: Board
end

Board.__index = Board

function Board.new(w: integer, h: integer) : Board
   local self : Board = setmetatable({}, Board as metatable<Board>)
   self.w, self.h = w, h
   self.wires = {}
   self.components = {}
   assert(type(self.w) == 'number' and type(self.h) == 'number')
   return self
end

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

function Board:add_component(def: ComponentDef, pos: Position, dir: Direction)
   assert(dir ~= CENTER)

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

function Board:rotate_tile(pos: Position)
   local component = self:component(pos)
   if component and component.def.can_rotate then
      component.direction = fdir.rotation[component.direction]
   end
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

function Board:render() : Scene
   return {
	  -- { command = "draw", image = "board_top_left", x = 0, y = 0 },
   }
end

--[[
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
