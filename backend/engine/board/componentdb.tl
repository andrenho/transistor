local type ComponentDB = record
end

function ComponentDB.new() : ComponentDB
   local self : ComponentDB = setmetatable({}, { __index = ComponentDB })
   return self
end

return ComponentDB
