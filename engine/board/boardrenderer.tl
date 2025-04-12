require "ui.scene"
require "ui.context"
require "board.board"
require "board.geo.position"
require "board.wire"

local record Cache
   tile: ResourceId
   board_top: ResourceId
   board_bottom: ResourceId
   board_left: ResourceId
   board_right: ResourceId
end

global record BoardRenderer
   board: Board
   cache: Cache
   __index: BoardRenderer
end
BoardRenderer.__index = BoardRenderer

local TILE_SIZE = 16

local function load_cache() : Cache
   return {
	  tile = cache_image("tile"),
	  board_top = cache_image("board_top"),
	  board_bottom = cache_image("board_bottom"),
	  board_right = cache_image("board_right"),
	  board_left = cache_image("board_left"),
   }
end

function BoardRenderer.new(board: Board) : BoardRenderer
    local self : BoardRenderer = setmetatable({}, BoardRenderer as metatable<BoardRenderer>)
	self.board = board
    return self
end

function BoardRenderer:add_image(scene: Scene, image: string | ResourceId, x: integer, y: integer, context?: Context)
   scene:add_image(image, math.floor(x * TILE_SIZE + (self.board.x / self.board.zoom)), math.floor(y * TILE_SIZE + (self.board.y / self.board.zoom)), context)
end

function BoardRenderer:__draw_board(scene: Scene)
   local C = self.cache
   self:add_image(scene, "board_top_left", -2, -2)
   self:add_image(scene, "board_top_right", self.board.w, -2)
   self:add_image(scene, "board_bottom_left", -2, self.board.h)
   self:add_image(scene, "board_bottom_right", self.board.w, self.board.h)
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
   local wire_image = function(pos: Position, wire: Wire, value: integer) : string
	  local v : string; if value > 0 then v = "on" else v = "off" end
	  return "wire_" .. wire.layer .. "_" .. v .. "_" .. pos.dir:lower() .. "_" .. wire.width
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

function BoardRenderer:__draw_components(scene: Scene)
   -- components
   for _, component in ipairs(self.board.components) do
      local context : Context = { 
		 position = { 
			x = math.floor(component.position.x * TILE_SIZE + (self.board.x / self.board.zoom)),
			y = math.floor(component.position.y * TILE_SIZE + (self.board.y / self.board.zoom)),
		 },
		 rotation = Dir.angle[component.direction]
	  }
	  component.def.render(component, scene, context)
   end
   -- shadow component
   if self.board.cursor.tile and self.board.cursor.selected_component ~= "" then
      local context : Context = { 
		 position = { 
			x = math.floor(self.board.cursor.tile.x * TILE_SIZE + (self.board.x / self.board.zoom)),
			y = math.floor(self.board.cursor.tile.y * TILE_SIZE + (self.board.y / self.board.zoom)),
		 },
		 rotation = Dir.angle[self.board.cursor.selected_direction],
		 opacity = .5,
	  }
	  local def = self.board.component_db:def(self.board.cursor.selected_component)
	  def.render(nil, scene, context)
   end
end

function BoardRenderer:render(wires_values: {integer, integer}) : Scene
   if not self.cache then
	  self.cache = load_cache()
   end
   local scene = Scene.new()
   scene:push_context({ zoom = self.board.zoom })
   self:__draw_board(scene)
   self:__draw_wires(scene, wires_values)
   self:__draw_components(scene)
   return scene
end

-- vim:sw=3:st=3:sts=3:noexpandtab

