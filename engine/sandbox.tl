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
   window_w: integer
   window_h: integer
   moving_device: integer
end

function Sandbox.new() : Sandbox
   local self : Sandbox = setmetatable({}, { __index = Sandbox })
   self.board = Board.new(20, 10)
   self.devices = {self.board}
   self.selected_device = 1
   self.moving_device = nil
   self.component_db = ComponentDB.new()
   self.last_x, self.last_y = 0, 0
   self.window_w, self.window_h = 0, 0
   return self
end

function Sandbox:render() : {Scene}
   return {self.board:render()}
end

function Sandbox:__device_at_point(x: integer, y: integer) : integer
   -- TODO
   return 1
end

function Sandbox:__topmost_device() : integer
   -- TODO
   return 1
end

function Sandbox:__bring_device_to_front(dev_idx: integer)
   -- TODO
end

function Sandbox:move_device(idx: integer, rel_x: integer, rel_y: integer)
   local dev = self.devices[idx]
   local rect = dev:rect()
   -- local min_x = -- TODO

   self.devices[idx].x = self.devices[idx].x + rel_x
   self.devices[idx].y = self.devices[idx].y + rel_y
end

function Sandbox:do_event(event: Event)
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

   -- select another device?
   if event.type == "mouse_press" then
	  local dev_at_cursor = self:__device_at_point(event.x, event.y)
	  if dev_at_cursor ~= self:__topmost_device() then
		 self:__bring_device_to_front(dev_at_cursor)
	  end
   end

   -- move device
   if event.type == "mouse_press" and event.button == "right" then
	  self.moving_device = self:__device_at_point(event.x, event.y)
   elseif event.type == "mouse_release" and event.button == "right" then
	  self.moving_device = nil
   elseif event.type == "move_pointer" and self.moving_device then
	  self:move_device(self.moving_device, rel_x, rel_y)
   end
end

return Sandbox

-- vim:sw=3:st=3:sts=3:noexpandtab
