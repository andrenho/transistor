local type Scene = require "ui.scene"
local type Event = require "event"
local type Device = require "device"

local ComponentDB = require "components.componentdb"
local Board = require "board.board"

local record Sandbox
   board: Board
   devices: {Device}
   selected_device: integer
   component_db: ComponentDB
   last_x: integer
   last_y: integer
end

function Sandbox.new() : Sandbox
   local self : Sandbox = setmetatable({}, { __index = Sandbox })
   self.board = Board.new(20, 10)
   self.devices = {self.board}
   self.selected_device = 1
   self.component_db = ComponentDB.new()
   return self
end

function Sandbox:render() : {Scene}
   return {self.board:render()}
end

function Sandbox:do_event(event: Event)
   -- register last position
   if event.type == "move_pointer" then
	  self.last_x, self.last_y = event.x, event.y
   end
end

return Sandbox

-- vim:sw=3:st=3:sts=3:noexpandtab
