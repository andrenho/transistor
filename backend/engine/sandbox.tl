local Board = require 'board'

local type Sandbox = record
    boards: {number: Board}
    board_id_counter: number
    -- component_db
end

function Sandbox.new() : Sandbox
   local self : Sandbox = setmetatable({}, { __index = Sandbox })
   self.boards = {}
   self.board_id_counter = 1
   return self
end

function Sandbox:add_board(w: number, h: number) : Board
    local board = Board.new(self.board_id_counter, w, h, self)
    self.boards[self.board_id_counter] = board
    self.board_id_counter = self.board_id_counter + 1
    return board
end

return Sandbox
