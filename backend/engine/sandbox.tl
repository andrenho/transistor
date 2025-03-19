local type Sandbox = record
end

function Sandbox.new() : Sandbox
   local self : Sandbox = setmetatable({}, { __index = Sandbox })
   return self
end

return Sandbox