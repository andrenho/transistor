local type Scene = require "ui.scene"

local ComponentDB = require "components.componentdb"
local Board = require "board.board"

local record Sandbox
   board: Board
   component_db: ComponentDB
end

function Sandbox.new() : Sandbox
   local self : Sandbox = setmetatable({}, { __index = Sandbox })
   self.board = Board.new(20, 10)
   self.component_db = ComponentDB.new()
   return self
end

function Sandbox:render() : {Scene}
   return {self.board:render()}
end

return Sandbox

-- vim:sw=3:st=3:sts=3:noexpandtab
