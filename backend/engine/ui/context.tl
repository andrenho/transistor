local record Context
   rotation: number
   opacity: number

   __index: Context
end
Context.__index = Context

function Context.new() : Context
    local self : Context = setmetatable({}, Context as metatable<Context>)
    return self
end

return Context

-- vim:sw=3:st=3:sts=3:noexpandtab

