local record ContextPos
   x: integer
   y: integer
end

local record ContextRect
   x: integer
   y: integer
   w: integer
   h: integer
end

global record Context
   position: ContextRect
   rotation: number
   opacity: number
   zoom: number
   rotation_center: ContextPos
   draw_border: boolean
end

-- vim:sw=3:st=3:sts=3:noexpandtab

