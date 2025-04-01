local type Context = require "ui.context"

local enum Command
    "draw"
    "push_context"
    "pop_context"
end

-- draw:        { "draw", image, x, y, w, h, context }
-- push_context { "push_context", context }
-- pop_context  { "pop_context" }
local type Instruction = { any }

local record Scene is {Instruction}
   __index: Scene
end
Scene.__index = Scene

function Scene.new() : Scene
    local self : Scene = setmetatable({}, Scene as metatable<Scene>)
    return self
end

function Scene:add_image(image: string, x: integer, y: integer, context?: Context)
   self[#self+1] = { "draw", image, x, y, context }
end

function Scene:push_context(context: Context)
   self[#self+1] = { "push_context", context }
end

function Scene:pop_context()
   self[#self+1] = { "pop_context" }
end

return Scene

-- vim:sw=3:st=3:sts=3:noexpandtab

