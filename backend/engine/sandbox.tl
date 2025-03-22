local type Device = require "device"
local DeviceDB = require "devicedb"
local ComponentDB = require "components.componentdb"
local Board = require "board.board"
local Render = require "ui.render"

local record Sandbox
   device_db: DeviceDB
   devices: {DeviceId: Device}
   device_id_counter: DeviceId
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
   device.x = device.id * 30
   device.y = device.id * 30
   device.z_order = 0
   device.sandbox = self
   self.device_id_counter = self.device_id_counter + 1
   return device.id, device
end

function Sandbox:get_device(id: DeviceId) : Device
   return assert(self.devices[id])
end

function Sandbox:render() : Render
   -- order device list
   local devices : {Device} = {}
   for _,dev in pairs(self.devices) do devices[#devices+1] = dev end
   table.sort(devices, function(a: Device, b: Device) : boolean return a.z_order < b.z_order end)

   local render: Render = {}
   render.steps_per_second = 0
   for _,device in ipairs(devices) do
	  render.scenes[#render.scenes+1] = device:render()
   end
   return render
end

return Sandbox

-- vim:sw=3:st=3:sts=3:noexpandtab
