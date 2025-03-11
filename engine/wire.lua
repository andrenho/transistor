LAYER_TOP    = 'T'
LAYER_BOTTOM = 'B'

WIDTH_1 = '1'
WIDTH_8 = '8'

Wire = {}
Wire.__index = Wire

function Wire.new(layer, width)
   assert(layer == LAYER_TOP or layer == LAYER_BOTTOM)
   assert(width == WIDTH_1 or width == WIDTH_8)
   local self = setmetatable({}, Wire)
   self.layer = layer
   self.width = width
   return self
end

WR = Wire.new