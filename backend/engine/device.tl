local type SceneItem = require "ui.sceneitem"

global type Sandbox
global type DeviceId = integer

local interface Device
   -- interface definition
   name: string
   new: function(...: any) : Device
   render: function(self) : {SceneItem}

   -- local fields
   id: DeviceId
   x: integer
   y: integer
   z_order: integer
   sandbox: Sandbox
end

return Device

-- vim:sw=3:st=3:sts=3:noexpandtab
