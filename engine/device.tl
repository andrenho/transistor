local Rect = require "board.geo.rect"

local interface Device
   x: integer
   y: integer
   zoom: number
   z_order: integer

   size: function(self): (integer, integer)
   rect: function(self): Rect
end

return Device

-- vim:sw=3:st=3:sts=3:noexpandtab
