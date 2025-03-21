global enum WireLayer
    "top"
    "bottom"
end

global enum WireWidth
    "1"
    "8"
end

local record Wire
    layer: WireLayer
    width: WireWidth
end

function Wire.new(layer: WireLayer, width: WireWidth) : Wire
   return { layer = layer, width = width }
end

global WR = Wire.new

return Wire

-- vim:sw=3:st=3:sts=3:noexpandtab
