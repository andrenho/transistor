local type ComponentDef = record
end

function ComponentDef.new() : ComponentDef
   local self : ComponentDef = setmetatable({}, { __index = ComponentDef })
   return self
end

return ComponentDef
