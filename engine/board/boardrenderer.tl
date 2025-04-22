require "ui.scene"
require "ui.context"
require "board.board"
require "board.geo.position"
require "board.wire"

local type Cache = {string: ResourceId}

global record BoardRenderer
   board: Board
   cache: Cache
   __index: BoardRenderer
end
BoardRenderer.__index = BoardRenderer

local TILE_SIZE = 16

local function load_cache() : Cache
   local cache: Cache = {}
   -- prepare images
   local images = { "tile", "board_top", "board_bottom", "board_right", "board_left", "board_top_left", "board_top_right", "board_bottom_left", "board_bottom_right" }
   -- prepare ic
   local directions = { "n", "s", "w", "e" }
   local all_directions = { "nw", "n", "ne", "sw", "s", "se", "w", "e", "center" }
   for _,dir in ipairs(directions) do table.insert(images, "ic_dot_" .. dir) end
   for _,dir in ipairs(directions) do table.insert(images, "ic_pin_" .. dir) end
   for _,dir in ipairs(all_directions) do table.insert(images, "logic_gates_" .. dir) end
   for _,dir in ipairs(all_directions) do table.insert(images, "digital_" .. dir) end
   for _,dir in ipairs(all_directions) do table.insert(images, "memory_" .. dir) end
   for _,dir in ipairs(all_directions) do table.insert(images, "cpu_" .. dir) end
   -- prepare wires
   for _,side in ipairs({ "top" }) do
	  for _,value in ipairs({ "on", "off" }) do
		 for _,dir in ipairs(directions) do
			for _,width in ipairs({ "1" }) do
			   table.insert(images, "wire_" .. side .. "_" .. value .. "_" .. dir .. "_" .. width)
			end
		 end
	  end
   end
   -- cache all images
   for _, image in ipairs(images) do
	  cache[image] = cache_image(image)
   end
   return cache
end

function BoardRenderer.new(board: Board) : BoardRenderer
    local self : BoardRenderer = setmetatable({}, BoardRenderer as metatable<BoardRenderer>)
	self.board = board
    return self
end

function BoardRenderer:add_image(scene: Scene, image: ResourceId, x: integer, y: integer, context?: Context)
   scene:add_image(image, math.floor(x * TILE_SIZE + (self.board.x / self.board.zoom)), math.floor(y * TILE_SIZE + (self.board.y / self.board.zoom)), context)
end

function BoardRenderer:__draw_board(scene: Scene)
   local C = self.cache
   self:add_image(scene, C.board_top_left, -2, -2)
   self:add_image(scene, C.board_top_right, self.board.w, -2)
   self:add_image(scene, C.board_bottom_left, -2, self.board.h)
   self:add_image(scene, C.board_bottom_right, self.board.w, self.board.h)
   for x=0,self.board.w-1 do
	  self:add_image(scene, C.board_top, x, -2)
	  self:add_image(scene, C.board_bottom, x, self.board.h)
   end
   for y=0,self.board.h-1 do
	  self:add_image(scene, C.board_left, -2, y)
	  self:add_image(scene, C.board_right, self.board.w, y)
   end
   for x=0,self.board.w-1 do
	  for y=0,self.board.h-1 do
		 self:add_image(scene, C.tile, x, y)
	  end
   end
end

function BoardRenderer:__draw_wires(scene: Scene, wires_values: {integer, integer})
   local wire_image = function(pos: Position, wire: Wire, value: integer) : ResourceId
	  local v : string; if value > 0 then v = "on" else v = "off" end
	  return self.cache["wire_" .. wire.layer .. "_" .. v .. "_" .. pos.dir:lower() .. "_" .. wire.width]
   end
   -- wires
   for pos_hash, wire in pairs(self.board.wires) do
	  local pos = Position.unhash(pos_hash)
	  self:add_image(scene, wire_image(pos, wire, wires_values[pos_hash] or 0), pos.x, pos.y)
   end
   --temporary wire
   if self.board.cursor.wire then
	  local w = self.board.cursor.wire
	  for _,pos in ipairs(Position.a_to_b(w.starting_pos, w.end_pos, w.orientation)) do
		 self:add_image(scene, wire_image(pos, self.board.cursor.selected_wire, 0), pos.x, pos.y, { opacity = .5 })
	  end
   end
end

function BoardRenderer:__draw_component_single_tile(scene: Scene, component: Component | nil, def: ComponentDef, x: integer, y: integer, dir: Direction, semitransparent: boolean)
   local context : Context = { 
	  position = { 
		 x = math.floor(x * TILE_SIZE + (self.board.x / self.board.zoom)),
		 y = math.floor(y * TILE_SIZE + (self.board.y / self.board.zoom)),
	  },
	  rotation = Dir.angle[dir],
	  opacity = semitransparent and 0.5 or 1,
   }
   def.render(component, scene, context)
end

function BoardRenderer:__draw_ic(scene: Scene, component: Component, def: ComponentDef, x: integer, y: integer, dir: Direction, semitransparent: boolean)
   local C = self.cache
   local context : Context = { opacity = semitransparent and 0.5 or 1 }
   local cat = def.category
   local r = def:rect(P(x, y), dir)

   -- shell
   self:add_image(scene, C[cat .. "_nw"], r.top_left.x, r.top_left.y, context)
   self:add_image(scene, C[cat .. "_ne"], r.bottom_right.x, r.top_left.y, context)
   self:add_image(scene, C[cat .. "_sw"], r.top_left.x, r.bottom_right.y, context)
   self:add_image(scene, C[cat .. "_se"], r.bottom_right.x, r.bottom_right.y, context)
   for tx=(r.top_left.x + 1), (r.bottom_right.x - 1) do
	  self:add_image(scene, C[cat .. "_n"], tx, r.top_left.y, context)
	  self:add_image(scene, C[cat .. "_s"], tx, r.bottom_right.y, context)
	  for ty=(r.top_left.y + 1), (r.bottom_right.y - 1) do
		 self:add_image(scene, C[cat .. "_center"], tx, ty, context)
	  end
   end
   for ty=(r.top_left.y + 1), (r.bottom_right.y - 1) do
	  self:add_image(scene, C[cat .. "_w"], r.top_left.x, ty, context)
	  self:add_image(scene, C[cat .. "_e"], r.bottom_right.x, ty, context)
   end

   -- dot
   if dir == N then self:add_image(scene, C.ic_dot_n, r.top_left.x + 1, r.top_left.y + 1, context)
   elseif dir == E then self:add_image(scene, C.ic_dot_e, r.top_left.x + 1, r.bottom_right.y - 1, context)
   elseif dir == S then self:add_image(scene, C.ic_dot_s, r.bottom_right.x - 1, r.bottom_right.y - 1, context)
   elseif dir == W then self:add_image(scene, C.ic_dot_w, r.bottom_right.x - 1, r.top_left.y + 1, context)
   end

   -- pins
   local pin_pos = def:pin_positions(P(x, y), dir)
   for _,p in ipairs(pin_pos) do
	  if p.pos.y <= r.top_left.y then
		 self:add_image(scene, C.ic_pin_n, p.pos.x, p.pos.y, context)
	  elseif p.pos.y >= r.bottom_right.y then
		 self:add_image(scene, C.ic_pin_s, p.pos.x, p.pos.y, context)
	  elseif p.pos.x <= r.top_left.x then
		 self:add_image(scene, C.ic_pin_e, p.pos.x, p.pos.y, context)
	  elseif p.pos.x >= r.bottom_right.x then
		 self:add_image(scene, C.ic_pin_w, p.pos.x, p.pos.y, context)
	  end
   end

   -- custom rendeer
   local fx : number = x
   local fy : number = y
   if dir == N or dir == S then 
	  fx = x + 0.5
   else
	  fy = y + 0.5
   end
   local context_render : Context = {
	  opacity = semitransparent and 0.5 or 1,
	  position = { 
		 x = math.floor((fx - 1) * TILE_SIZE + (self.board.x / self.board.zoom)),
		 y = math.floor((fy - 1) * TILE_SIZE + (self.board.y / self.board.zoom)),
		 w = math.floor((r.bottom_right.x - r.top_left.x - 1) * TILE_SIZE * self.board.zoom),
		 h = math.floor((r.bottom_right.y - r.top_left.y - 1) * TILE_SIZE * self.board.zoom),
	  },
	  rotation = Dir.angle[Dir.rotation[dir]],
   }
   def.render(component, scene, context_render)
end

function BoardRenderer:__draw_component(scene: Scene, component: Component, def: ComponentDef, x: integer, y: integer, dir: Direction, semitransparent: boolean)
   if def.type == "single_tile" then
	  self:__draw_component_single_tile(scene, component, def, x, y, dir, semitransparent)
   else
	  self:__draw_ic(scene, component, def, x, y, dir, semitransparent)
   end
end

function BoardRenderer:__draw_pin_names(component: Component)
   local r = component:rect()
   for _,p in ipairs(component:pin_positions()) do
	  local x, y: integer, integer
	  local text: string
	  if p.pos.y <= r.top_left.y then  -- top
		 -- self:add_image(scene, C.ic_pin_n, p.pos.x, p.pos.y, context)
	  elseif p.pos.y >= r.bottom_right.y then  -- bottom
		 -- self:add_image(scene, C.ic_pin_s, p.pos.x, p.pos.y, context)
	  elseif p.pos.x <= r.top_left.x then  -- left
		 -- self:add_image(scene, C.ic_pin_e, p.pos.x, p.pos.y, context)
	  elseif p.pos.x >= r.bottom_right.x then  -- right
		 -- self:add_image(scene, C.ic_pin_w, p.pos.x, p.pos.y, context)
		 --scene:add_image(image, math.floor(x * TILE_SIZE + (self.board.x / self.board.zoom)), math.floor(y * TILE_SIZE + (self.board.y / self.board.zoom)), context)
		 text = "AR"
		 x = p.pos.x
		 y = p.pos.y
	  end
	  if text then
		 scene:add_text({

		 })
	  end
   end
end

function BoardRenderer:__draw_components(scene: Scene, tile_x: integer, tile_y: integer)
   -- components
   for _, component in ipairs(self.board.components) do
	  self:__draw_component(scene, component, component.def, component.position.x, component.position.y, component.direction, false)
	  if component:rect():contains(P(tile_x, tile_y)) then
		 self:__draw_pin_names(component)
	  end
   end
   -- shadow component
   if self.board.cursor.tile and self.board.cursor.selected_component ~= "" then
	  local def = self.board.component_db:def(self.board.cursor.selected_component)
	  local rect = def:rect(P(self.board.cursor.tile.x, self.board.cursor.tile.y), self.board.cursor.selected_direction)
	  if rect:fully_inside_of(Rect.new(0, 0, self.board.w - 1, self.board.h - 1)) then
		 self:__draw_component(scene, nil, def, self.board.cursor.tile.x, self.board.cursor.tile.y, self.board.cursor.selected_direction, true)
	  end
   end
end

function BoardRenderer:render(wires_values: {integer, integer}, last_x: integer, last_y: integer) : Scene
   local tile_x = math.floor((last_x - self.board.x) / TILE_SIZE / self.board.zoom)
   local tile_y = math.floor((last_y - self.board.y) / TILE_SIZE / self.board.zoom)
   if not self.cache then
	  self.cache = load_cache()
   end
   local scene = Scene.new()
   scene:push_context({ zoom = self.board.zoom })
   self:__draw_board(scene)
   self:__draw_wires(scene, wires_values)
   self:__draw_components(scene, tile_x, tile_y)
   return scene
end

-- vim:sw=3:st=3:sts=3:noexpandtab

