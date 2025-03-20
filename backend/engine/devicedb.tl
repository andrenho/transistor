local type Device = require('device')

local record DeviceDB
    items: {string: Device}
end

function DeviceDB.new() : DeviceDB
   local self : DeviceDB = setmetatable({}, { __index = DeviceDB })
   self.items = {}
   return self
end

function DeviceDB:get(name: string): Device
    local dev = self.items[name]
    if dev == nil then
        error("Device `" .. name .. "` not found.")
    end
    return dev
end

function DeviceDB:validate_device(dev: Device)
    assert(dev.name)
    assert(dev.new)
end

function DeviceDB:add_native(dev: Device) : Device
    self:validate_device(dev)
    self.items[dev.name] = dev
    return dev
end

return DeviceDB
