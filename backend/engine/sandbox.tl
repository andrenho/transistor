local type Device = require('device')
local DeviceDB = require('devicedb')
local ComponentDB = require('components.componentdb')
local Board = require('board.board')

local record Sandbox
   device_db: DeviceDB
   devices: {number: Device}
   device_id_counter: number
   component_db: ComponentDB
end

function Sandbox.new() : Sandbox
   local self : Sandbox = setmetatable({}, { __index = Sandbox })
   self.device_db = DeviceDB.new()
   self.device_db:add_native(Board)
   self.devices = {}
   self.device_id_counter = 1
   self.component_db = ComponentDB.new()
   return self
end

function Sandbox:add_device(name: string, ...: any) : number, Device
   local dev_type = self.device_db:get(name)
   local device = dev_type.new(...)
   self.devices[self.device_id_counter] = device
   device.id = self.device_id_counter
   device.sandbox = self
   self.device_id_counter = self.device_id_counter + 1
   return device.id, device
end

function Sandbox:get_device(id: number) : Device
   return assert(self.devices[id])
end

function Sandbox:render() : Render

return Sandbox

-- vim:sw=3:st=3:sts=3:noexpandtab
