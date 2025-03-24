local type Scene = require "ui.scene"

local ComponentDB = require "components.componentdb"
local Board = require "board.board"

local record Sandbox
   boards: {BoardId: Board}
   board_id_counter: BoardId
   component_db: ComponentDB
end

function Sandbox.new() : Sandbox
   local self : Sandbox = setmetatable({}, { __index = Sandbox })
   self.boards = {}
   self.board_id_counter = 1
   self.component_db = ComponentDB.new()
   return self
end

function Sandbox:add_board(w: integer, h: integer): number, Board
   local board = Board.new(w, h)
   self.boards[self.board_id_counter] = board
   board.id = self.board_id_counter
   board.x = (board.id + 1) * 30
   board.y = (board.id + 1) * 35
   board.z_order = 0
   board.zoom = 2
   board.sandbox = self
   self.board_id_counter = self.board_id_counter + 1
   return board.id, board
end

function Sandbox:get_board(id: BoardId) : Board
   return assert(self.boards[id])
end

function Sandbox:render() : {Scene}
   -- order device list
   local boards: {Board} = {}
   for _,board in pairs(self.boards) do boards[#boards+1] = board end
   table.sort(boards, function(a: Board, b: Board) : boolean return a.z_order < b.z_order end)

   local scenes : {Scene} = {}
   for _,board in ipairs(boards) do
	   scenes[#scenes+1] = board:render()
   end
   return scenes
end

return Sandbox

-- vim:sw=3:st=3:sts=3:noexpandtab
