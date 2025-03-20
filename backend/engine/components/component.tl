local record Component
   data: {integer: integer}  -- TODO
   __index: Component
end
Component.__index = Component

function Component.new() : Component
    local self : Component = setmetatable({}, Component as metatable<Component>)
    return self
end

return Component

-- vim:sw=3:st=3:sts=3:noexpandtab
