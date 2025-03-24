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

local record Context
   position: ContextRect
   rotation: number
   opacity: number
   zoom: number
   rotation_center: ContextPos
end

return Context

-- vim:sw=3:st=3:sts=3:noexpandtab

