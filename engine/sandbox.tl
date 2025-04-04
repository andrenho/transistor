require "ui.scene"
require "event"
require "device"
require "board.boardrenderer"
require "components.componentdb"
require "board.board"

global record Sandbox
   board: Board
   devices: {Device}
   selected_device: integer
   component_db: ComponentDB
   last_x: integer
   last_y: integer
   window_w: integer
   window_h: integer
   moving_device: integer
   board_renderer: BoardRenderer
end

global record SerialSandbox
   board: SerialBoard
   component_db: SerialComponentDB
end

function Sandbox.new() : Sandbox
   local self : Sandbox = setmetatable({}, { __index = Sandbox })
   self.component_db = ComponentDB.new()
   self.board = Board.new(20, 10, self.component_db)
   self.devices = {self.board}
   self.selected_device = 1
   self.moving_device = nil
   self.last_x, self.last_y = 0, 0
   self.window_w, self.window_h = 0, 0
   self.board_renderer = BoardRenderer.new(self.board)
   return self
end

function Sandbox:replace_board(board: Board)
   local old_board = self.board
   self.board = board
   self.board_renderer = BoardRenderer.new(board)
   for i=1,#self.devices do
	  if self.devices[i] == old_board then self.devices[i] = self.board end
   end
end

function Sandbox:serialize() : SerialSandbox
   return {
	  board = self.board:serialize(),
	  component_db = self.component_db:serialize(),
   }
end

function Sandbox.unserialize(data: SerialSandbox) : Sandbox
   local sandbox = Sandbox.new()
   sandbox.component_db = ComponentDB.unserialize(data.component_db)
   sandbox:replace_board(Board.unserialize(data.board, sandbox.component_db))
   return sandbox
end

function Sandbox:render() : {Scene}
   return {self.board_renderer:render()}
end

function Sandbox:__device_at_point(x: integer, y: integer) : integer
   for i,dev in ipairs(self.devices) do
	  if dev:rect():contains(P(x, y)) then
		 return i
	  end
   end
   return nil
end

function Sandbox:__topmost_device() : integer
   -- TODO
   return 1
end

function Sandbox:__bring_device_to_front(_: integer)
   -- TODO
end

local events_to_pass_always : {EventType:boolean} = {
   mouse_release=true, key_up=true, clear_board=true
}

local events_to_pass_when_under_pointer : {EventType:boolean} = {
   move_pointer=true, mouse_press=true, key_down=true,
}

function Sandbox:move_device(idx: integer, rel_x: integer, rel_y: integer)
   local dev = self.devices[idx]
   local size = dev:size()

   local min_x = math.floor(-size.w + 112 * dev.zoom)
   local min_y = math.floor(-size.h + 112 * dev.zoom)
   local max_x = math.floor(self.window_w - 56 * dev.zoom)
   local max_y = math.floor(self.window_h - 56 * dev.zoom)

   dev.x = math.min(math.max(dev.x + rel_x, min_x), max_x)
   dev.y = math.min(math.max(dev.y + rel_y, min_y), max_y)
end

function Sandbox:do_event(event: Event) : boolean
   local recompile = false
   local rel_x = 0
   local rel_y = 0

   -- window resize
   if event.type == "window_resize" then
	  self.window_w, self.window_h = event.x, event.y
   end

   -- register last position
   if event.type == "move_pointer" then
	  rel_x, rel_y = (event.x - self.last_x), (event.y - self.last_y)
	  self.last_x, self.last_y = event.x, event.y
   end

   local dev_at_cursor = self:__device_at_point(self.last_x, self.last_y)
   local device = self.devices[dev_at_cursor]

   -- select another device?
   if event.type == "mouse_press" then
	  if dev_at_cursor ~= self:__topmost_device() then
		 self:__bring_device_to_front(dev_at_cursor)
	  end
   end

   -- zoom
   if event.type == "mouse_wheel"  and device then
	  device.zoom = device.zoom + event.y
	  device.zoom = math.max(math.min(device.zoom, 3), 1)
   end

   -- move device
   if event.type == "mouse_press" and event.button == "right" then
	  self.moving_device = dev_at_cursor
   elseif event.type == "mouse_release" and event.button == "right" then
	  self.moving_device = nil
   elseif event.type == "move_pointer" and self.moving_device then
	  self:move_device(self.moving_device, rel_x, rel_y)
   end

   -- pass events to devices
   local topmost = self.devices[self:__topmost_device()]
   if topmost then
	  if events_to_pass_always[event.type] or (device and events_to_pass_when_under_pointer[event.type]) then
		 if topmost:do_event(event, self.last_x - topmost.x, self.last_y - topmost.y) then
			recompile = true
		 end
	  end
   end

   return recompile
end

-- vim:sw=3:st=3:sts=3:noexpandtab
