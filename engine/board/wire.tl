global enum WireLayer
    "top"
    "bottom"
end

global enum WireWidth
    "1"
    "8"
end

global record Wire
    layer: WireLayer
    width: WireWidth
   __index: Wire
end
Wire.__index = Wire

global type SerialWire = string

function Wire.new(layer: WireLayer, width: WireWidth) : Wire
   local self : Wire = setmetatable({}, Wire as metatable<Wire>)
   self.layer = layer
   self.width = width
   return self
end

function Wire.unserialize(data: SerialWire) : Wire
   return Wire.new(data:sub(1, 1) == "t" and "top" or "bottom", data:sub(2, 2) as WireWidth)
end

function Wire:serialize() : SerialWire
   return (self.layer == "top" and "t" or "b") .. self.width
end

global WR = Wire.new

-- vim:sw=3:st=3:sts=3:noexpandtab
