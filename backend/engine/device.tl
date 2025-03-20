global type Sandbox

local interface Device
   -- interface definition
   name: string
   new: function(...: any) : Device

   -- local fields
   id: number
   sandbox: Sandbox
end

return Device

-- vim:sw=3:st=3:sts=3:noexpandtab
