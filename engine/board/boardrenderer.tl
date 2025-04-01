local type Scene = require "ui.scene"
local type Context = require "ui.context"
local Board = require "board/board"
local Position = require "board.geo.position"
local fdir = require "board.geo.dir"
local Wire = require "board.wire"

local record Cache
   tile: ResourceId
end

local record BoardRenderer
   board: Board
   cache: Cache
   __index: BoardRenderer
end
BoardRenderer.__index = BoardRenderer

local TILE_SIZE = 16

local function load_cache() : Cache
   return {
	  tile = cache_image("tile")
   }
end

function BoardRenderer.new(board: Board) : BoardRenderer
    local self : BoardRenderer = setmetatable({}, BoardRenderer as metatable<BoardRenderer>)
	self.board = board
	self.cache = load_cache()
    return self
end

function BoardRenderer:add_image(scene: Scene, image: string, x: integer, y: integer, context?: Context)
   scene:add_image(image, math.floor(x * TILE_SIZE + (self.board.x / self.board.zoom)), math.floor(y * TILE_SIZE + (self.board.y / self.board.zoom)), context)
end

function BoardRenderer:__draw_board(scene: Scene)
   self:add_image(scene, "board_top_left", -2, -2)
   self:add_image(scene, "board_top_right", self.board.w, -2)
   self:add_image(scene, "board_bottom_left", -2, self.board.h)
   self:add_image(scene, "board_bottom_right", self.board.w, self.board.h)
   for x=0,self.board.w-1 do
	  self:add_image(scene, "board_top", x, -2)
	  self:add_image(scene, "board_bottom", x, self.board.h)
   end
   for y=0,self.board.h-1 do
	  self:add_image(scene, "board_left", -2, y)
	  self:add_image(scene, "board_right", self.board.w, y)
   end
   for x=0,self.board.w-1 do
	  for y=0,self.board.h-1 do
		 self:add_image(scene, "tile", x, y)
	  end
   end
end

function BoardRenderer:__draw_wires(scene: Scene)
   local wire_image = function(pos: Position, wire: Wire) : string
	  return "wire_" .. wire.layer .. "_off_" .. pos.dir:lower() .. "_" .. wire.width
   end
   -- wires
   for pos_hash, wire in pairs(self.board.wires) do
	  local pos = Position.unhash(pos_hash)
	  self:add_image(scene, wire_image(pos, wire), pos.x, pos.y)
   end
   --temporary wire
   if self.board.cursor.wire then
	  local w = self.board.cursor.wire
	  for _,pos in ipairs(Position.a_to_b(w.starting_pos, w.end_pos, w.orientation)) do
		 self:add_image(scene, wire_image(pos, self.board.cursor.selected_wire), pos.x, pos.y, { opacity = .5 })
	  end
   end
end

function BoardRenderer:__draw_components(scene: Scene)
   for _, component in ipairs(self.board.components) do
      local context = { 
		 position = { 
			x = math.floor(component.position.x * TILE_SIZE + (self.board.x / self.board.zoom)),
			y = math.floor(component.position.y * TILE_SIZE + (self.board.y / self.board.zoom)),
		 },
		 rotation = fdir.angle[component.direction]
	  }
	  component.def.render(component, scene, context)
   end
end

function BoardRenderer:render() : Scene
   local scene = Scene.new()
   scene:push_context({ zoom = self.board.zoom })
   self:__draw_board(scene)
   self:__draw_wires(scene)
   self:__draw_components(scene)
   return scene
end

return BoardRenderer

-- vim:sw=3:st=3:sts=3:noexpandtab

