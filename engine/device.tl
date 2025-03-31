local type Event = require "event"
local Rect = require "board.geo.rect"

global record Size
   w: integer
   h: integer
end

local interface Device
   x: integer
   y: integer
   zoom: number
   z_order: integer

   size: function(self): Size
   rect: function(self): Rect
   do_event: function(self, event: Event, rel_mouse_x: integer, rel_mouse_y: integer)
end

return Device

-- vim:sw=3:st=3:sts=3:noexpandtab
