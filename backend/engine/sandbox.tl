local type Device = require('device')
local Board = require('board.board')
local DeviceDB = require('devicedb')

local record Sandbox
   device_db: DeviceDB
   devices: {number: Device}
   device_id_counter: number
end

function Sandbox.new() : Sandbox
   local self : Sandbox = setmetatable({}, { __index = Sandbox })
   self.device_db = DeviceDB.new()
   self.device_db:add_native(Board)
   self.devices = {}
   self.device_id_counter = 1
   return self
end

function Sandbox:add_device(name: string, ...: any) : number
   local dev = self.device_db:get(name)
   self.devices[self.device_id_counter] = dev.new(...)
   dev.id = self.device_id_counter
   dev.sandbox = self
   self.device_id_counter = self.device_id_counter + 1
   return dev.id
end

function Sandbox:get_device(id: number) : Device
   return assert(self.devices[id])
end

return Sandbox

-- vim:sw=3:st=3:sts=3:noexpandtab
