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

global type SerialWire = string

function Wire.new(layer: WireLayer, width: WireWidth) : Wire
   return { layer = layer, width = width }
end

function Wire.unserialize(data: SerialWire) : Wire
   return Wire.new(data:sub(1, 1) == "t" and "top" or "bottom", data:sub(2, 2) as WireWidth)
end

function Wire:serialize() : SerialWire
   return self.layer == "top" and "t" or "b" .. self.width
end

global WR = Wire.new

return Wire

-- vim:sw=3:st=3:sts=3:noexpandtab
