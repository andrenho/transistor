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

global type WireSerial = string

function Wire.new(layer: WireLayer, width: WireWidth) : Wire
   return { layer = layer, width = width }
end

function Wire.unserialize(data: WireSerial) : Wire
   return Wire.new(data.layer, data.width)
end

function Wire:serialize() : WireSerial
end

global WR = Wire.new

return Wire

-- vim:sw=3:st=3:sts=3:noexpandtab
